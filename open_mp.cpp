#include <iostream>
#include <chrono>
#include <omp.h>
//#include <cstudio>
#include <cassert>
#include <stdlib.h>

int main(void){
  int sum = 0;
  std::chrono::duration<double> diff;
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < 1234567; i++){
    sum += i;
  }
  auto end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "SUM:  " << sum << " | TOOK: " << diff.count() << " sec" <<std::endl;
  
  int sum1 = 0;
  
  start = std::chrono::steady_clock::now();
#pragma omp parallel for reduction(+:sum1)
  for (int i = 0; i < 1234567; i++){
#pragma omp critical
    sum1 += i;
  }
  end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "SUM1: " << sum1 << " | TOOK: " << diff.count() << " sec" << std::endl;
  
  return 0;
}

