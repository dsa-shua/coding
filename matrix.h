/* ********************************************** */
/* CODE BY: DELA ROSA JOSHUA GABRIEL (임효진)       */
/* 2019142208                                     */
/* ELECTRICAL AND ELECTRONICS ENGINEERING         */
/* YONSEI UNIVERSITY                              */
/* github.com/dsa-shua                            */
/* ********************************************** */
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "matrix_data.h"

std::chrono::duration<double> diff;
std::vector<std::thread> threads;
const int n = 8;
const int m = 8;
const int size = 64;

const int num_threads = 4;
std::mutex global_lock;
int items_per_thread = size / num_threads;
int row_per_thread = n / num_threads;
int col_per_thread = m / num_threads;

void clear(void){
  threads.clear();
}
void dataSheet(void){
  // len of longest: 18
  printf("=========================\n");
  printf("n: %18d\n",n);
  printf("m: %18d\n",m);
  printf("size: %15d\n",size);
  printf("num_theads: %9d\n",num_threads);
  printf("items_per_thread: %3d\n",items_per_thread);
  printf("row_per_thread:   %3d\n", row_per_thread);
  printf("col_per_thread:   %3d\n", col_per_thread);
  printf("=========================\n");
}

void sanity_check(int* ref, int* target){
  int count = 0;
  for(int i = 0; i < n * m; i++){
    if(ref[i] != target[i]){
      if(count < 6){
        std::cout << "Error at [" << i << "] | REF: " << ref[i] << " TARGET: " << target[i] << std::endl;
      }
      count++;
    }
  }
  if (count != 0) std::cout << "Num Err: " << count << std::endl;
  else std::cout << "PASS" << std::endl;

}

void printMatrix(int* mat, int r, int c){
  for(int i = 0; i < r * c; i++){
    if ((i % c) == 0) printf("\n");
    printf("%3d ", mat[i]);
  }
  printf("\n");
}

void serial_matmul(int* m1, int* m2, int* m3, int r, int c){
  std::chrono::duration<double> diff;
  auto time_start = std::chrono::steady_clock::now();
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++){
        m3[i*r + j] += m1[i*c + k] * m2[k*r + j];
      }
        
  auto time_end = std::chrono::steady_clock::now();
  diff = time_end - time_start;
  std::cout << "Serial took: " << diff.count() << " sec" << std::endl;
}

void serial_transpose(int* target, int* destination, int r, int c){
  std::chrono::duration<double> diff;
  auto time_start = std::chrono::steady_clock::now();
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
        destination[j*r + i] = target[i*c + j];
  auto time_end = std::chrono::steady_clock::now();
  diff = time_end - time_start;
  std::cout << "Serial Tranpose Took: " << diff.count() << " sec" << std::endl;
}

void _transpose(int* target, int* destination, int r, int c, int tid){
  int start_row = tid*row_per_thread;
  int end_row = start_row + row_per_thread;
  int start = tid*items_per_thread;
  int end = start + items_per_thread;
  
  for (int i = start_row; i < end_row;i++)
    for(int j = 0; j < c; j++)
      destination[j*r + i] = target[i*c + j];
  
}

void transpose(int* target, int* destination, int r, int c){
  std::chrono::duration<double> diff;
  auto time_start = std::chrono::steady_clock::now();
  for(int i = 0; i < r; i++)
    threads.push_back(std::thread(_transpose, target, destination, r, c, i));
  
  for(auto& thread: threads)
    thread.join();
  threads.clear();
  
  auto time_end = std::chrono::steady_clock::now();
  diff = time_end - time_start;
  std::cout << "Tranpose took: " << diff.count() << " sec" << std::endl;
  
}

void col_wise(int tid, int* m1, int* m2, int* m3, int r, int c){
  int start_col = tid*col_per_thread;
  int end_col = start_col + col_per_thread;
  int start = start_col;
  int end = 0;
  for(int i = 0; i < r; i++){
    for(int j = start_col; j < end_col; j++){
      for(int k = 0; k < r; k++){
        m3[i*r + j] += m1[i*c + k] * m2[k*r+ j];
      }
    }
  }
}

void row_wise(int tid, int* m1, int* m2, int* m3, int r, int c){
  int start_row = tid*row_per_thread;
  int end_row = start_row + row_per_thread;
  int start = tid*items_per_thread;
  int end = start + items_per_thread;

  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        m3[i*r + j] += m1[i*c + k] * m2[k*r+ j];
      }
    }
  }
}

void parallel_row(int* m1, int* m2, int* m3, int r, int c){
  auto time_start = std::chrono::steady_clock::now();
  for(int tid = 0; tid < num_threads; tid++){
    threads.push_back(std::thread(row_wise, tid, m1,m2,m3,r,c));
  }
  for(auto& thread: threads){
    thread.join();
  }
  
  auto time_end = std::chrono::steady_clock::now();
  diff = time_end - time_start;
  std::cout << "Row Parallel Took: " << diff.count() << " sec" << std::endl;
}

void parallel_col(int* m1, int* m2, int* m3, int r, int c){
  auto time_start = std::chrono::steady_clock::now();
  for(int tid = 0; tid < num_threads; tid++){
    threads.push_back(std::thread(col_wise, tid,m1,m2,m3,r,c));
  }
  for(auto& thread: threads){
    thread.join();
  }
  
  auto time_end = std::chrono::steady_clock::now();
  diff = time_end - time_start;
  std::cout << "Col Parallel Took: " << diff.count() << " sec"<< std::endl;
}

int* generate_matrix(int rows, int cols){
  int* matrix = new int[rows*cols];
  srand(time(NULL)); // generate random seed
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      matrix[i * cols + j] = rand() % 6; // generate between 0 and 5
    }
  }
  return matrix;
}

int* generate_0(int rows, int cols){
  // zeros generator (for empty)
  int* matrix = new int[rows*cols];
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      matrix[i * cols + j] = 0;
    }
  }
  return matrix;
}

void do_transpose(int* mat1, int* mat2, int* mat3, int r, int c){
  
}

void parallel_transpose(int* mat1, int* mat2, int* mat3, int r, int c){
  int* transposed_b = generate_0(r,c);
  serial_transpose(transposed_b,mat2,r,c);
}
