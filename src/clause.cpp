#include "./headers/clause.h"

using namespace std;

Clause::Clause(){
}


Clause::~Clause(){
}

void Clause::setFormat(string f, int nb, int nc){
		fmt = f;
		num_bvar = nb;
		num_cls = nc;
}

void Clause::insertClause(vector<int>& c){
}

Clause::clause_ Clause::getClause(int i){
}

int Clause::getNumBvar(){
	return num_bvar;
}

int Clause::getNumCls(){
	return num_cls;
}

void Clause::printClausesInfo(){
	cout << "========================================================" << endl;
	cout << "The input format is " << fmt << endl;
    cout << "Number of binary variables is " << num_bvar << endl;
    cout << "Number of clauses is " << num_cls << endl;
    cout << "========================================================" << endl;
}