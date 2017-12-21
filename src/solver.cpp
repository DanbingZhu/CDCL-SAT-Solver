#include "./headers/solver.h"

Solver::Solver()
{
	this->clauses.clear();

	this->assignment_set.clear(); // assignment mapping

	this->decision_stack.clear();

	this->singletons.clear();
	this->inGamma.clear(); // track clauses in Gamma(singletons), avoid duplicates

	decision_level = 0;
	this->pure_literals.clear();
	implication_graph.clear();
}

Solver::~Solver(){
}

void Solver::insertClause(vector<int>& clause){
	clause_ temp;
	if(clause.size() == 1){
		temp.content = clause;
		//for a singleton, we don't care about the watch literals
		//temp.watch.first;
		//temp.watch.second;
	} else{
		temp.content = clause;
		// set the watch literals to the first two literals
		temp.watch.first = clause[0];
		temp.watch.second = clause[1];
	}
    
	clauses.push_back(temp);
}

int Solver::CDCL(time_t timer, int time_limit){
	time_t now;
	pureLiteralElimination();
	initSingletons();
	while(true){
		// unitPropagation returns true if there is NO CONFLICT
		if(unitPropagation()){
			if(allAssigned()){
				cout << "sat" << endl;
				return 1;
			} else {
				pair<int, bool> new_decision = decisionMaking();
				if(new_decision.first != -1){
					decision_level = decision_level + 1; // increse decision_level
					decision_stack.insert(new_decision.first); // add to decision stack
					assignment_set[new_decision.first] = new_decision.second; // update assignment map
					decision_level_map[new_decision.first] = decision_level; // update decision level map

					vector<int> empty; // just an empty vector
					addNode(1, empty,new_decision); // update implication graph

					addToGamma(new_decision.first); // add relavent clauses to singletons vector
				}
			}
		} else{ // there is a conflict
			// d,c <- analyzeAndLearn(sigma, G)
			LearningResult learnt = analyzeAndLearn(timer, time_limit);
			if(learnt.backtrack_level < 0){
				cout << "unsat" << endl;
				return 0;
			} else{
				// phi <- phi AND c
				insertClause(learnt.conflict_clause);
				num_cls = clauses.size(); // update hte number of clauses

				// do backtracking
				backtrack(learnt.backtrack_level);
			}
		}

		time(&now);
		if(difftime(now, timer) >= time_limit){
			cout << "unknown" << endl;
            break;
        }
	}
	return (-1);
}

LearningResult Solver::analyzeAndLearn(time_t timer, int time_limit){
	vector<int> conflict_clause; // used to store the learnt clause
	int second_highest_decision_level; // the second highest decision level, to which we will backtrack
	set<int> seen_parents;
	int max_decision_level_count = 0;
	deque<int> reason_queue;
	vector<int> levels;
	time_t now;

	if(decision_level == 0){
		vector<int> empty;
		LearningResult result(-1, empty);
		return result;
	}

	// get reasoning queue
	for(auto p : implication_graph[num_bvar+1]){
		reason_queue.push_back(p);
	}

	while(!reason_queue.empty()){
		time(&now);
		if(difftime(now, timer) >= time_limit){
			cout << "unknown" << endl;
			printDecision();
            exit(-1);
        }
		for(auto n : reason_queue){
			// if the node is already counted
			if(seen_parents.find(n) != seen_parents.end()){
				continue;
			}
			else{
				seen_parents.insert(n);
			}

			if(decision_level_map[n] == decision_level){
				max_decision_level_count++;
			}
		}

		seen_parents.clear();

		if(max_decision_level_count == 1){
			break;
		} else {
			int victim = reason_queue.front();
			reason_queue.pop_front(); 
			if(decision_stack.find(victim) == decision_stack.end() && decision_level_map[victim] == decision_level){
				for(auto n : implication_graph[victim]){
					reason_queue.push_back(n);
				}
			}
			else{
				// it's a decision node, put it back into the reason queue
				reason_queue.push_back(victim);
			}
			max_decision_level_count = 0;
		}
	}

	seen_parents.clear();

	// assemble conflict clause
	for(auto n : reason_queue){
		if(seen_parents.find(n) != seen_parents.end()){
			continue;
		}
		else{
			seen_parents.insert(n);
			levels.push_back(decision_level_map[n]);
		}

		if(assignment_set[n] == false){
			conflict_clause.push_back(n);
		}
		else{
			conflict_clause.push_back(-n);
		}
	}

	sort(levels.begin(), levels.end());


	// get the second highest decision level
	second_highest_decision_level = findSecondHighest(levels);

	last_learnt_clause = conflict_clause;
	
	LearningResult result(second_highest_decision_level, conflict_clause);
	return result;
}

int Solver::findSecondHighest(vector<int>& levels){

	if(levels.size() == 1){
		return 0;
	}

	for(int i = levels.size() - 1; i > 0; i--){
		if(levels[i] < decision_level){
			if(levels[i] == 0){
				return (-1); // unsat
			}
			else{
				return levels[i];
			}
		}
	}

	return levels[0];
}


// add all clauses relevant to var to the Gamma stack
void Solver::addToGamma(int var){
	for(int i = 0; i < num_cls; i++){
		if(clauses[i].content.size() > 1){
			for(int j = 0; j < clauses[i].content.size(); j++){
				if(abs(clauses[i].content[j]) == var){
					if(inGamma.find(clauses[i].content) == inGamma.end()){ // check if the clause is already in there
						singletons.push_back(clauses[i]);
						inGamma.insert(clauses[i].content);
	                    break;
					}
					
				}
			}
		}
	}
}

void Solver::pureLiteralElimination(){
	// initialize ;
	vector<polarity_> polarity(num_bvar+1);
	for(int i = 1; i <= num_bvar; i++){
		polarity[i].pos = false;
		polarity[i].neg = false;
		pure_literals.insert(i); // at first, put all the literals in this vector, those non-pure_literals will be erased in the following loop
	}
	// check all the literals in all clauses
	for(int i = 0; i < num_cls; i++){
		for(int j = 0; j < clauses[i].content.size(); j++){
			if(clauses[i].content[j] > 0){
				polarity[clauses[i].content[j]].pos = true;
			} else {
				polarity[-clauses[i].content[j]].neg = true;
			}
			if(polarity[abs(clauses[i].content[j])].pos && polarity[abs(clauses[i].content[j])].neg){ // both positive and negative polarity of the literal have appeared, it's not pure literal
                if(pure_literals.find(abs(clauses[i].content[j])) != pure_literals.end()){
                    pure_literals.erase(abs(clauses[i].content[j]));
                }
			}
		}
	}
    // go through all the clauses again to clean out the ones contain pure literals
	for(int i = 0; i < num_cls; i++){
		for(int j = 0; j < clauses[i].content.size(); j++){
			if(pure_literals.find(abs(clauses[i].content[j])) != pure_literals.end()){
				if(clauses[i].content[j] > 0){
					assignment_set[clauses[i].content[j]] = true;
				} else {
					assignment_set[-clauses[i].content[j]] = false;
				}
			}
		}
	}
}

void Solver::initSingletons(){
	for(int i = 0; i < num_cls; i++){
		if(clauses[i].content.size() == 1 && pure_literals.find(clauses[i].content[0]) == pure_literals.end()){
			singletons.push_back(clauses[i]);
			inGamma.insert(clauses[i].content);
		}
	}
}

bool Solver::unitPropagation(){
	while(true){
		vector<int> clause = pickClause();
		if(clause.size() == 0){ // NULL
			return true;
		} else if(clause[0] == -(num_bvar+1)){ // conflict;
			return false;
		} else if(clause.size() == 1){ // singleton
			if(clause[0] < 0){
				assignment_set[-clause[0]] = false;
				decision_level_map[-clause[0]] = decision_level; //should be zero
				decision_stack.insert(-clause[0]);
				vector<int> empty;
				pair<int, bool> singleton_decision = make_pair(-clause[0], false);
				addNode(1, empty, singleton_decision); // it's a decision at level 0
				addToGamma(-clause[0]);
			}
			else{
				assignment_set[clause[0]] = true;
				decision_level_map[clause[0]] = decision_level; //should be zero
				decision_stack.insert(clause[0]);
				vector<int> empty;
				pair<int, bool> singleton_decision = make_pair(clause[0], false);
				addNode(1, empty, singleton_decision); // it's a decision at level 0
				addToGamma(clause[0]);
			}
		} else{
			pair<int, bool> new_assignment = UnitRule(clause);
			if(new_assignment.first != -1){
				assignment_set[new_assignment.first] = new_assignment.second;
				decision_level_map[new_assignment.first] = decision_level;
				addNode(2, clause, new_assignment);

				addToGamma(new_assignment.first);
			}
		}
	}
}

void Solver::addNode(int type, vector<int>& clause, pair<int, bool>& assignment){
	if(type == 0){ // it's a conflict
		for(int i = 0; i < clause.size(); i++){
			implication_graph[num_bvar+1].push_back(abs(clause[i])); // num_bvar+1 to represent conflict in the graph
		}
	}
	else if(type == 1){ // it's a decision
		implication_graph[assignment.first].push_back(assignment.first); // loop back to itself
	}
	else if(type == 2){ // it's an implied assignment
		for(int i = 0; i < clause.size(); i++){
			if(abs(clause[i]) != assignment.first && decision_level_map[abs(clause[i])] != 0){
				implication_graph[assignment.first].push_back(abs(clause[i])); // parents are literals other than itself and not pure literals
			}
		}
	}
	else{
		cout << "ERROR: unsupported assignment type";
		exit(-1);
	}
}

pair<int, bool> Solver::UnitRule(vector<int> clause){
	for(int i = 0; i < clause.size(); i++){
		if(assignment_set.find(abs(clause[i])) == assignment_set.end()){
			if(clause[i] > 0){
				return make_pair(clause[i], true);
			} else {
				return make_pair(-clause[i], false);
			}
		}
	}
	pair<int, bool> no_decision = make_pair(-1, false);
	return no_decision;
}

bool Solver::allAssigned(){
	if(assignment_set.size() == num_bvar){
		return true;
	} else {
		return false;
	}
}

pair<int, bool> Solver::decisionMaking(){
	for(int i = 1; i <= num_bvar; i++){
		if(assignment_set.find(i) == assignment_set.end()){
			pair<int, bool> new_decision = make_pair(i, true);
			return new_decision;
		}
	}
	pair<int, bool> no_decision = make_pair(-1, false);
	return no_decision;
}

void Solver::backtrack(int backtrack_level){
	set<int> to_eliminate;
	int decision;

	for(auto dl : decision_level_map){
		if(dl.second == backtrack_level && decision_stack.find(dl.first) != decision_stack.end()){
			decision = dl.first;
			continue; // if it is the decision at backtrack level, don't touch
		}
		if(dl.second >= backtrack_level){
			to_eliminate.insert(dl.first);
		}
	}

	for(auto v : to_eliminate){
		decision_stack.erase(v);
		implication_graph.erase(v);
		assignment_set.erase(v);
		decision_level_map.erase(v);
	}

	decision_level = backtrack_level;
	if(backtrack_level > 0){
		addToGamma(decision);
	}
	else{
		initSingletons();
	}
}

vector<int> Solver::pickClause(){
	while(!singletons.empty()){
		clause_ c = singletons.front();
		vector<int> clause = c.content;
		singletons.pop_front();
		inGamma.erase(clause);
		if(clause.size() == 1){
			if(assignment_set.find(abs(clause[0])) != assignment_set.end()){
				if(clause[0] < 0 && assignment_set[-clause[0]] == true){
					vector<int> conflict;
					conflict.push_back(-(num_bvar+1));
					return conflict;
				} 
				if(clause[0] > 0 && assignment_set[clause[0]] == false){
					vector<int> conflict;
					conflict.push_back(-(num_bvar+1));
					return conflict;
				}
			}
			return clause;
		}

		int w1 = c.watch.first;
		int w2 = c.watch.second;
		if((assignment_set.find(abs(w1)) != assignment_set.end() && w1 > 0 && assignment_set[abs(w1)] == true) || (assignment_set.find(abs(w2)) != assignment_set.end() && w2 > 0 && assignment_set[abs(w2)] == true) || (assignment_set.find(abs(w1)) != assignment_set.end() && w1 < 0 && assignment_set[abs(w1)] == false) || (assignment_set.find(abs(w2)) != assignment_set.end() && w2 < 0 && assignment_set[abs(w2)] == false)){
			continue;
		}
		else if(assignment_set.find(abs(w1)) == assignment_set.end()){
			if(assignment_set.find(abs(w2)) == assignment_set.end()){
				continue;
			} else{
				if(moveWatch(c, 2)){
					continue;
				} else {
					return clause;
				}
			}
		}
		else {
			if(moveWatch(c, 1)){
				singletons.push_front(c);
				inGamma.insert(clause);
				continue;
			} else{
				if(assignment_set.find(abs(w2)) == assignment_set.end()){
					return clause;
				} else {

					//add conflict node
					pair<int, bool> empty_assignment;
					addNode(0, clause, empty_assignment); // 0 indicates it is a conflict

					singletons.clear();
					inGamma.clear();
					vector<int> conflict;
					conflict.push_back(-(num_bvar+1));
					return conflict;
				}
			}
		}
	}
	vector<int> N;
	return N; // return NULL
}

bool Solver::moveWatch(clause_& c, int idx){
	for(int i = 0; i < c.content.size(); i++){
		// skip the already watched ones
		if(c.content[i] == c.watch.first || c.content[i] == c.watch.second){
			continue;
		}
		if(assignment_set.find(abs(c.content[i])) == assignment_set.end() || (assignment_set[abs(c.content[i])] == true && c.content[i] > 0) || (assignment_set[abs(c.content[i])] == false && c.content[i] < 0)){
			if(idx == 1){
				c.watch.first = c.content[i];
				return true;
			} 
			if(idx == 2){
				c.watch.second = c.content[i];
				return true;
			}
		}
	}
	return false;
}

void Solver::printAssignment(){
	for(auto i : assignment_set){
		cout << i.first << " " << i.second << endl;
	}
}

void Solver::printClause(vector<int>& clause){
	for(auto i : clause){
		cout << i << " ";
	}
	cout << endl;
}

void Solver::printImplicationGraph(){
	for(auto n : implication_graph){
		cout << "[" << n.first  << "]" << endl;

		for(auto p : n.second){
			cout << p << " ";
		}
		cout << endl;
	}
}


void Solver::printDecision(){
	for(auto d : decision_stack){
		cout << d << " ";
	}
	cout << endl;
}

void Solver::printLastLearntClause(){
	printClause(last_learnt_clause);
}
