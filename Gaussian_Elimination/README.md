# Gaussian Elimination in Parallel
## Overview
- This program demonstrates that certain tasks do not translate well into parallel.
- We developed a C program that will do gaussian elimination in parallel
- The forward elimination and the backsolving are implemented and measured seperately.
- We run the function on input sizes of random matrices of sizes 10, 100, 1000 to see how the program scales with input. 
 
## Usage

1. run `make` in the terminal to compile the program
2. run `./gaussian_elimination` to execute the program
3. there are no command line args everything is hard coded.

### Example execution
```bash
ziadarafat@dogearchvm ~/D/s/parallel-programming (main)> cd Gaussian_Elimination/
ziadarafat@dogearchvm ~/D/s/p/Gaussian_Elimination (main)> make
gcc -O0 -Wall -Werror -Wextra -pedantic -pthread -lpthread -o gaussian_elimination gaussian_elimination.c
ziadarafat@dogearchvm ~/D/s/p/Gaussian_Elimination (main)> ./gaussian_elimination
Time for forward elimination N=10: 0.001633 seconds
Time taken for backsolve with N=10: 0.000352 seconds
Time for forward elimination N=100: 0.095059 seconds
Time taken for backsolve with N=100: 0.003582 seconds
Time for forward elimination N=1000: 12.931657 seconds
Time taken for backsolve with N=1000: 0.034218 seconds

```