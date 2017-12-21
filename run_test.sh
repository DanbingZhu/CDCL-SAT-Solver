#!/bin/bash

echo "SAT: " > SAT.txt
echo "UNSAT: " > UNSAT.txt
echo "TIMEOUT: " > UNKNOWN.txt

for cnf_file in ./testcases/*.cnf
do
    ./bin/solver "$cnf_file" --time 30 > temp.txt
    result=$( tail -n 1 temp.txt )
if [ "$result" == "sat" ]; then
    echo "$cnf_file" >> SAT.txt
fi
if [ "$result" == "unsat" ]; then
    echo "$cnf_file" >> UNSAT.txt
fi
if [ "$result" == "unknown" ]; then
    echo "$cnf_file" >> UNKNOWN.txt
fi
done
