/*
Ziad Arafat
08/26/2021

- Created file

This file will fill an array with random numbers between 0 and 3 inclusive.
It will then use a serial function to count the number of 3s in the function.
After that we will begin to implement the parallel version.



*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct rand_three {
  int array[20];
};

struct rand_three rand_arr(struct rand_three my_arr) {
  struct rand_three x = my_arr;
  for (int i = 0; i < 20; i++) {
    rand_three->array[i] = 0;
  }
}

int main(int argc, char const *argv[]) {
  srand(time(0));
  // Create an empty array
  // set it equal to rand_arr(my_arr)
  // set count to count_3_serial()
  // print the result
  return 0;
}
