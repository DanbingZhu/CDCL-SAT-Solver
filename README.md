# CSE291 Automatic Reasoning
## DPLL SAT Solver

To build:
```
make
```
To clean:
```
make clean
```
To run single test case:
```
./solver <input_filename> [--time timelimit(sec)] [--verbose]
```
I have made a bash script to run the CDCL solver on all the petie test cases. To run:
```
(optional)chmod +x run_test
./run_test.sh
```
### Notes:
* Petie test cases are not included in the Github repository(can be found at https://github.com/dreal/sat-benchmarks/tree/master/petite), to run the bash script, put them into a directory named as **testcases**. Otherwise, you need to modify the run_test.sh file accordingly.
* The duration of each run is set to about 30 seconds. The results are stored in three .txt files, namely SAT.txt, UNSAT.txt and UNKNOWN.txt. I will also list them below.
* The overall performance is better than the DPLL solver. However, I found that for total-order-8 and total-order-9, they are unable to be solved by the CDCL solver while they were solved by DPLL solver.

## Results
### SAT:
graph-ordering-alt-10.cnf |
graph-ordering-alt-11.cnf |
graph-ordering-alt-12.cnf |
graph-ordering-alt-13.cnf |
graph-ordering-alt-14.cnf |
graph-ordering-alt-15.cnf |
graph-ordering-alt-16.cnf |
graph-ordering-alt-17.cnf |
graph-ordering-alt-18.cnf |
graph-ordering-alt-19.cnf |
graph-ordering-alt-2.cnf |
graph-ordering-alt-3.cnf |
graph-ordering-alt-4.cnf |
graph-ordering-alt-5.cnf |
graph-ordering-alt-6.cnf |
graph-ordering-alt-7.cnf |
graph-ordering-alt-8.cnf |
graph-ordering-alt-9.cnf |
order-principle-alt-10.cnf |
order-principle-alt-11.cnf |
order-principle-alt-12.cnf |
order-principle-alt-13.cnf |
order-principle-alt-14.cnf |
order-principle-alt-15.cnf |
order-principle-alt-16.cnf |
order-principle-alt-17.cnf |
order-principle-alt-18.cnf |
order-principle-alt-19.cnf |
order-principle-alt-2.cnf |
order-principle-alt-3.cnf |
order-principle-alt-4.cnf |
order-principle-alt-5.cnf |
order-principle-alt-6.cnf |
order-principle-alt-7.cnf |
order-principle-alt-8.cnf |
order-principle-alt-9.cnf |
total-order-alt-10.cnf |
total-order-alt-11.cnf |
total-order-alt-12.cnf |
total-order-alt-13.cnf |
total-order-alt-14.cnf |
total-order-alt-15.cnf |
total-order-alt-16.cnf |
total-order-alt-17.cnf |
total-order-alt-18.cnf |
total-order-alt-19.cnf |
total-order-alt-2.cnf |
total-order-alt-3.cnf |
total-order-alt-4.cnf |
total-order-alt-5.cnf |
total-order-alt-6.cnf |
total-order-alt-7.cnf |
total-order-alt-8.cnf |
total-order-alt-9.cnf

### UNSAT:
graph-ordering-11.cnf |
graph-ordering-12.cnf |
graph-ordering-13.cnf |
graph-ordering-10.cnf |
graph-ordering-14.cnf |
graph-ordering-15.cnf |
graph-ordering-16.cnf |
graph-ordering-17.cnf |
graph-ordering-18.cnf |
graph-ordering-19.cnf |
graph-ordering-2.cnf |
graph-ordering-3.cnf |
graph-ordering-4.cnf |
graph-ordering-5.cnf |
graph-ordering-6.cnf |
graph-ordering-7.cnf |
graph-ordering-8.cnf |
graph-ordering-9.cnf |
total-order-2.cnf |
total-order-3.cnf |
total-order-4.cnf |
total-order-5.cnf |
total-order-6.cnf |
total-order-7.cnf

### UNKNOWN:
total-order-10.cnf |
total-order-11.cnf |
total-order-12.cnf |
total-order-13.cnf |
total-order-14.cnf |
total-order-15.cnf |
total-order-16.cnf |
total-order-17.cnf |
total-order-18.cnf |
total-order-19.cnf |
total-order-8.cnf |
total-order-9.cnf

