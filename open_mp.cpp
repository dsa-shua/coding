#include <iostream>
#include <chrono>
#include <omp.h>
//#include <cstudio>
#include <cassert>
#include <stdlib.h>

int main(void){
  
  
  omp_set_num_threads(10);
  
#pragma omp parallel sections
  {
    
#pragma omp section
    {
      for(int i = 0; i < 100;i++){
        int tid = omp_get_thread_num();
        printf("Thread ID: %2d section A\n", tid);
      }
    } // no barriers after each section
    
#pragma omp section
    {
      for (int i = 0; i < 100; i++){
        int tid = omp_get_thread_num();
        printf("Thread ID: %2d section B\n", tid);
      }
    }
  }
  
  
  return 0;
}

