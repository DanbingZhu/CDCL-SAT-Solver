#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>
#include <iostream>
#include <string>
using namespace std;

class Clause {
protected:
	string fmt;
	int num_bvar;
	int num_cls;

	struct clause_ {
		vector<int> content;
		pair<int, int> watch;
	};

	struct polarity_ {
		bool pos;
		bool neg;
	};

public:
	Clause();
	~Clause();

	void setFormat(string f, int nb, int nc);

	// function to insert clauses
	virtual void insertClause(vector<int>& c);

	// function to get the ith clause
	virtual clause_ getClause(int i);

	int getNumBvar();

	int getNumCls();

	// function to print basic infomations about the clauses
	void printClausesInfo();
};

#endif