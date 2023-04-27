#include "matmul.h"
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>

#define DEBUG 0
#define SERIAL 0

void matmul_ref(const int* const matrixA, const int* const matrixB,
                int* const matrixC, const int n, const int m) {
  // You can assume matrixC is initialized with zero
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      for (int k = 0; k < m; k++)
        matrixC[i * n + j] += matrixA[i * m + k] * matrixB[k * n + j];
}

void panic(){
  std::cout << "panic" << std::endl;
  while(1) {;}
}


void row_wise(const int* const A, const int* const B, int* const C, const int n, const int m, int start, int i){
  // start will tell what we will start with
  int end = start + 128;
#ifndef DEBUG
  printf("Start:[%2d] %4d to %4d\n",i, start, end);
#endif

  for(int i = start; i < end; i++)
    for(int j = 0; j < n; j++)
      for(int k = 0; k < m; k++)
        C[i * n + j] += A[i * m + k] * B[k * n + j];
}


void col_wise(const int* const A, const int* const B, int* const C, const int n, const int m, int row, int col){
  for(int k = 0; k < m; k++){
    C[row*n + col] += A[row * m + k] * B[k * n + col];
  }
}

void process(void) {
  
  
}

void matmul_optimized(const int* const matrixA, const int* const matrixB,
                      int* const matrixC, const int n, const int m) {
  // TODO: Implement your code
  
#ifndef SERIAL
  int* serial = new int[n * n];
  std::chrono::duration<double> diff;
  auto start = std::chrono::steady_clock::now();
  matmul_ref(matrixA, matrixB, serial, n,m);
  auto end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "Serial took: " << diff.count() << std::endl;
#endif
  
    
  const int num_threads = 16;
  const int num_elements = n * m;
  const int num_cols = n;
  const int num_rows = m;
  const int col_per_thread = n / num_threads;
  const int row_per_thread = m / num_threads;


#ifndef DEBUG
  printf("\n");
  std::cout << "num threads: " << num_threads << std::endl;
  std::cout << "num_elements: " << num_elements << std::endl;
  std::cout << "num_cols: " << num_cols << std::endl;
  std::cout << "num_rows: " << num_rows << std::endl;
  std::cout << "size of matrix: " << num_cols << " x " << num_rows << std::endl;
  std::cout << "col_per_thread: " << col_per_thread << std::endl;
  std::cout << "row_per_thread: " << row_per_thread << std::endl;
  
  printf("mem(A): %p\n", matrixA);
  printf("mem(B): %p\n", matrixB);
  printf("mem(C): %p\n", matrixC);
#endif

  std::vector<std::thread> threads;
  // Row-Wise Matrix Multiplication
  {
    // check what each thread will work on
#ifndef DEBUG
    for(int i = 0; i < num_threads+1; i++){
      printf("[%d]: %d\n", i, i*row_per_thread);
    }
#endif

#ifndef SERIAL
    start = std::chrono::steady_clock::now();
    
#endif
    for(int i = 0; i < num_threads; i++){
      threads.push_back(std::thread(row_wise, matrixA, matrixB, matrixC, n, m, i*row_per_thread, i));
  
    }

    for(auto& thread: threads){
      thread.join();
    }

#ifndef SERIAL
    end = std::chrono::steady_clock::now();
    diff = end - start;
    std::cout << "Parallel Took: " << diff.count() << std::endl;
    panic();
#endif
  }

#ifndef DEBUG
  std::cout << "Finished Task" << std::endl;
#endif
//  panic();
}
