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
#pragma omp single
    {
      for(int i  = 0; i < 10; i++){
        int tid = omp_get_thread_num();
        printf("Thread ID: %d section A\n", tid);
      }
    } // Implicit Barrier
    
    
#pragma omp for
    for(int i = 0; i < 100; i++){
      int tid = omp_get_thread_num();
      printf("Thread ID: %d section B\n", tid);
    }
  }
  
  
  return 0;
}

