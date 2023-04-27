#include <iostream>
#include <chrono>
#include <omp.h>
//#include <cstudio>
#include <cassert>
#include <stdlib.h>

int main(void){
  
  int is_private = -2;
  
#pragma omp parallel firstprivate(is_private)
  {
    int tid= omp_get_thread_num();
    printf("TID: %2d | is_private(before) = %d\n", tid, is_private);
    is_private = tid;
    printf("TID: %2d | is_private(after) = %d\n", tid, is_private);
  }
  
  printf("Main Thread | is_private = %d\n", is_private);

  return 0;
}

