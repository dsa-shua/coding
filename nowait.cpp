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
      
      //#pragma omp for nowait
    #pragma omp for nowait
    for (int i = 0; i < 10; i++){
      printf("TID: %2d | section A \n", omp_get_thread_num());
    }

    
    #pragma omp for
    for (int i = 0; i < 10; i++){
      printf("TID: %2d | section B \n", omp_get_thread_num());
    }
  }

  
  return 0;
}

