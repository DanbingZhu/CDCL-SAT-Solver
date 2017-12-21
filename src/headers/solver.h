#ifndef SLOVER_H
#define SLOVER_H

#include "clause.h"
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <cstdlib>
#include <cstdint>
#include <ctime>

struct LearningResult{
	int backtrack_level;
	vector<int> conflict_clause;

	LearningResult(int b_l, vector<int>& c_c): backtrack_level(b_l), conflict_clause(c_c){}
};

class Solver : public Clause{
protected:
	vector<clause_> clauses;

	map<int, bool> assignment_set; // assignment mapping

	deque<clause_> singletons;
	set<vector<int>> inGamma; // track clauses in Gamma(singletons), avoid duplicates

	set<int> pure_literals;


	int decision_level;
	set<int> decision_stack; // record all the decisions
	unordered_map<int, int> decision_level_map; // recode the decision level of all the assignment
	unordered_map<int, vector<int>> implication_graph; // store the implication graph
	vector<int> last_learnt_clause;
	
public:
	Solver();
	~Solver();

	// override the insertClause in base Solver class, put the singletons to singletons vector
	void insertClause(vector<int>&);

	// DPLL sat solver
	int CDCL(time_t, int);


	LearningResult analyzeAndLearn(time_t, int);

	void addNode(int, vector<int>&, pair<int, bool>&);

	int findSecondHighest(vector<int>&);

	// add all clauses relevant to var to the Gamma stack
	void addToGamma(int);

	// function to eliminate pure literals and clauses that contain them
	void pureLiteralElimination();

	void initSingletons();

	// unit propagation
	bool unitPropagation();

	pair<int, bool> UnitRule(vector<int>);

	// check if all the variables are assigned a value
	bool allAssigned();

	pair<int, bool> decisionMaking();

	void backtrack(int);

	// function to pick clause
	vector<int> pickClause();

	bool moveWatch(clause_&, int);

	void printAssignment();

	void printClause(vector<int>&);

	void printImplicationGraph();

	void printDecision();

	void printLastLearntClause();

};


#endif
