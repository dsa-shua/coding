#include <iostream>
#include <chrono>
#include <omp.h>
//#include <cstudio>
#include <cassert>
#include <stdlib.h>

int main(void){

  omp_set_num_threads(10);
  
#pragma omp parallel
  {
    
#pragma omp master
    {
      for(int i = 0; i < 100; i++){
        int tid = omp_get_thread_num();
        printf("Thread ID: %2d | section A\n", tid);
      }
    }
#pragma omp barrier

#pragma omp for
    for (int i = 0; i < 100; i++){
      int tid = omp_get_thread_num();
      printf("Thread ID: [%2d] | section B", tid);
    }
  }

  
  return 0;
}

