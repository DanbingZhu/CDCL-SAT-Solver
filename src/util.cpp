#include "./headers/util.h"

using namespace std;

vector<int> split(const string &s, char delim){
    stringstream ss(s);
    vector<int> items;
    string item;
    while(getline(ss, item, delim)){
        items.push_back(stoi(item));
    }
    
    return items;
}

void parseHeader(const string &s, char delim, string& fmt, int& num_bvar, int& num_clause){
    stringstream ss(s);
    int count = 0;
    string item;
    while(getline(ss, item, delim)){
        switch(count){
            case 1: fmt = item; break;
            case 2: num_bvar = stoi(item); break;
            case 3: num_clause = stoi(item); break;
        }
        count++;
    }
    return;
}