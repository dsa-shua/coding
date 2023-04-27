#include <iostream>
#include <chrono>
#include <omp.h>
//#include <cstudio>
#include <cassert>
#include <stdlib.h>

int main(void){
  
  int last_private = -2;
  /*
   First private: copies the first instance outside the region
   Modifying is_private within parallel block does not modify the value outside
    the block.
   
   
   Last Private: keep final value to the main or original value
    becomes whats written in the last iteration, no matter when,
    or which thread executed it.
   
   Open MP knows which is the last iteration hence the last private is always
   the same for each iteration
   
   
   We can do both: initialize private data using firstprivate,
   then save the very last data on the last by using lastprivate
   */
#pragma omp parallel for firstprivate(last_private) lastprivate(last_private)
  for (int i = 0; i <10; i++)
  {
    int tid= omp_get_thread_num();
    printf("TID: %2d executing i = %d | is_private(before) = %d\n", tid, i, last_private);
    last_private = i;
    printf("TID: %2d executing i = %d | is_private(after) = %d\n", tid, i, last_private);
    assert(last_private == i);
  }
  
  printf("Main Thread | is_private = %d\n", last_private);

  return 0;
}

