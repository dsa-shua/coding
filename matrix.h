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
#include <random>
#include "matrix_data.h"

void printMatrix(int* mat, int r, int c);
std::chrono::duration<double> diff;
std::vector<std::thread> threads;
const int n = 512 << 2;
const int m = 512 << 2;
int size = n*m;

int num_threads = 8;
std::mutex global_lock;
int items_per_thread = size / num_threads;
int row_per_thread = n / num_threads;
int col_per_thread = m / num_threads;


void mat_add(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r*c; i++)
    mat3[i] = mat1[i] + mat2[i];
}

void p_mat_add(int tid, int* mat1, int* mat2, int* mat3, int r, int c){
  std::unique_lock<std::mutex> lock(global_lock);
  int start = tid*r; // starting column
  int end = (tid+1)*c*r;
  
  for(int i = start; i < end; i++)
    mat3[i] = mat1[i] + mat2[i];
}

void hold(void){
  printf("HOLD...\n");
  while(1) {;}
}

void clear(void){
  threads.clear();
}
void dataSheet(void){
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

void sanity_check(int* ref, int* target, int r, int c){
  int count = 0;
  for(int i = 0; i < r * c; i++){
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
  
  for (int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      destination[j*r + i] = target[i*c + j];
}

void transpose(int* target, int* destination, int r, int c){
//  std::chrono::duration<double> diff;
//  auto time_start = std::chrono::steady_clock::now();
  threads.clear();
  for(int i = 0; i < num_threads; i++)
    threads.push_back(std::thread(_transpose, target, destination, r, c, i));
  
  for(auto& thread: threads)
    thread.join();
  threads.clear();
  
//  auto time_end = std::chrono::steady_clock::now();
//  diff = time_end - time_start;
//  std::cout << "Tranpose took: " << diff.count() << " sec" << std::endl;
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
  
  int adjust = 0;
  if(num_threads <= r) {
    num_threads = num_threads >> 1; //adjust to prevent error
    adjust = 1;
  }
  
  for(int tid = 0; tid < num_threads; tid++){
    threads.push_back(std::thread(row_wise, tid, m1,m2,m3,r,c));
  }
  for(auto& thread: threads){
    thread.join();
  }
  
  if (adjust) num_threads = num_threads << 1; // revert
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
//  srand(time(NULL)); // generate random seed
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

void do_transpose(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = tid*row_per_thread;
  int end_row = start_row + row_per_thread;
  for(int i = start_row; i < end_row; i++)
    for(int j = 0; j < c; j++)
      for(int k = 0; k < r; k++)
        mat3[i*r + j] += mat1[i*r + k] * mat2[j*c + k];

}

void parallel_transpose(int* mat1, int* mat2, int* mat3, int r, int c){
  std::chrono::duration<double> diff;
  auto time_start = std::chrono::steady_clock::now();
  int* transposed_b = generate_0(r,c);
  transpose(mat2,transposed_b,r,c);
  
  for (int i = 0; i < num_threads; i++){
    threads.push_back(std::thread(do_transpose,mat1,transposed_b,mat3,r,c,i));
  }
  for(auto& thread: threads){
    thread.join();
  }
  
  auto time_end = std::chrono::steady_clock::now();
  diff = time_end - time_start;
  std::cout << "Tranpose MATMUL Took: " << diff.count() << " sec" << std::endl;
}

void __b_do(int tid, int* m1, int* m2, int* m3, int r, int c){
  // redefine due to change in r and c
  std::unique_lock<std::mutex> lock(global_lock);
  
  int _row_per_thread = r / num_threads;
  int _size = r * c;
  int _items_per_thread = _size / num_threads;
  int start_row = tid*_row_per_thread;
  int end_row = start_row + _row_per_thread;
  int start = tid*_items_per_thread;
  int end = start + _items_per_thread;

  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        m3[i*r + j] += m1[i*c + k] * m2[k*r+ j];
      }
    }
  }
}

void _b_plug_C0(int* target, int* destination,int r, int c){
  // r, c: midpoint value
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++){
      destination[i*r*2 + j] = target[i*r + j];
    }
}

void _b_plug_C1(int* target, int* destination,int r, int c){
  // r, c: midpoint value
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++){
      destination[i*r*2 + j+c] = target[i*r + j];
    }
}

void _b_plug_C2(int* target, int* destination,int r, int c){
  // r, c: midpoint value
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++){
      destination[(i+r)*r*2 + j] = target[i*r + j];
    }
}

void _b_plug_C3(int* target, int* destination,int r, int c){
  // r, c: midpoint value
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++){
      destination[(i+r)*r*2 + j+c] = target[i*r + j];
    }
}

void _b_matmul(int* m1, int* m2, int* m3, int r, int c){
  int adjust = 0;
  if(num_threads > r){
    while(num_threads > r)
      num_threads = num_threads >> 1;
    adjust++;
  }
    
  for(int tid = 0; tid < num_threads; tid++){
    threads.push_back(std::thread(__b_do, tid, m1,m2,m3,r,c));
  }
  for(auto& thread: threads){
    thread.join();
  }
  threads.clear();
  
  // revert
  if (adjust) num_threads = 16;
}

void _b_sub0(int* target, int* dest, int r2, int c2, int tid){
  int start = tid*(r2/2); // starting row
  int end = start + (r2/2); // ending row

  for(int i = start; i < end;i++) // just half of the block rows
    for(int k = 0; k < c2; k++){ // full cols (of block)
      dest[i*r2+ k] = target[i*(r2*2) + k];
    }
}

void _b_sub1(int* target, int* dest, int r2, int c2, int tid){
  int start = tid*(r2/2); // starting row
  int end = start + (r2/2); // ending row

  for(int i = start; i < end;i++) // just half of the block rows
    for(int k = c2; k < c2*2; k++){ // full cols (of block)
      dest[i*r2+ k-c2] = target[i*(r2*2) + k];
    }
}

void _b_sub2(int* target, int* dest, int r2, int c2, int tid){
  int start = tid*(r2/2); // starting row
  int end = start + (r2/2); // ending row

  for(int i = start; i < end;i++) // just half of the block rows
    for(int k = 0; k < c2; k++){ // full cols (of block)
      dest[i*r2+ k] = target[(i + r2)*(r2*2) + k];
    }
}

void _b_sub3(int* target, int* dest, int r2, int c2, int tid){
//  std::unique_lock<std::mutex> lock(global_lock);
  int start = tid*(r2/2); // starting row
  int end = start + (r2/2); // ending row

  for(int i = start; i < end;i++) // just half of the block rows
    for(int k = c2; k < c2*2; k++){ // full cols (of block)
      dest[i*r2+ k-c2] = target[(i + r2)*(r2*2) + k];
    }
}


void blocked_matrix(int* mat1, int* mat2, int* mat3, int r, int c){
  std::chrono::duration<double> diff;
  auto start = std::chrono::steady_clock::now();
  // checking thread count
  if((num_threads % 4)!= 0){
    printf("\n\npanic: num thread error\n");
    printf("num_threads: %d\n", num_threads);
    while(1) {;}
  }
  
  int threads_per_block = num_threads / 4;
  int row_midpoint = r / 2;
  int col_midpoint = c / 2;
  
  int* A0 = generate_0(row_midpoint, col_midpoint);
  int* A1 = generate_0(row_midpoint, col_midpoint);
  int* A2 = generate_0(row_midpoint, col_midpoint);
  int* A3 = generate_0(row_midpoint, col_midpoint);
  int* B0 = generate_0(row_midpoint, col_midpoint);
  int* B1 = generate_0(row_midpoint, col_midpoint);
  int* B2 = generate_0(row_midpoint, col_midpoint);
  int* B3 = generate_0(row_midpoint, col_midpoint);
    
  // Block Assignment
  threads.clear();
  for(int i = 0; i < 2; i++){
    // A BLOCKS
    threads.push_back(std::thread(_b_sub0, mat1, A0, row_midpoint, col_midpoint, i));
    threads.push_back(std::thread(_b_sub1, mat1, A1, row_midpoint, col_midpoint, i));
    threads.push_back(std::thread(_b_sub2, mat1, A2, row_midpoint, col_midpoint, i));
    threads.push_back(std::thread(_b_sub3, mat1, A3, row_midpoint, col_midpoint, i));
    
    
    // B BLOCKS
    threads.push_back(std::thread(_b_sub0, mat2, B0, row_midpoint, col_midpoint, i));
    threads.push_back(std::thread(_b_sub1, mat2, B1, row_midpoint, col_midpoint, i));
    threads.push_back(std::thread(_b_sub2, mat2, B2, row_midpoint, col_midpoint, i));
    threads.push_back(std::thread(_b_sub3, mat2, B3, row_midpoint, col_midpoint, i));
  }
  
  // Wait for block assignment
  for(auto& thread: threads){
    thread.join();
  }
  threads.clear();
  
  // Adjust Memory Allocation
  delete[] mat1;
  delete[] mat2;
  
  // Actual Matrix Multiplication
  int* C0 = generate_0(row_midpoint, col_midpoint);
  int* C1 = generate_0(row_midpoint, col_midpoint);
  int* C2 = generate_0(row_midpoint, col_midpoint);
  int* C3 = generate_0(row_midpoint, col_midpoint);
  
  {
    int* buffer1 = generate_0(row_midpoint, col_midpoint);
    int* buffer2 = generate_0(row_midpoint, col_midpoint);
    
    _b_matmul(A0,B0,buffer1,row_midpoint,col_midpoint);
    _b_matmul(A1,B2,buffer2, row_midpoint,col_midpoint);
  
    std::vector<std::thread> local_threads;
    for(int tid = 0; tid < num_threads; tid++)
      local_threads.push_back(std::thread(p_mat_add,tid,buffer1,buffer2,C0,row_midpoint/num_threads,col_midpoint));
    
    for(auto& thread: local_threads)
      thread.join();

    delete[] buffer1;
    delete[] buffer2;
  }
  
  {
    int* buffer1 = generate_0(row_midpoint, col_midpoint);
    int* buffer2 = generate_0(row_midpoint, col_midpoint);
    
    _b_matmul(A0,B1,buffer1,row_midpoint,col_midpoint);
    _b_matmul(A1,B3,buffer2, row_midpoint,col_midpoint);
    
    std::vector<std::thread> local_threads;
    for(int tid = 0; tid < num_threads; tid++)
      local_threads.push_back(std::thread(p_mat_add,tid,buffer1,buffer2,C1,row_midpoint/num_threads,col_midpoint));
    
    for(auto& thread: local_threads)
      thread.join();
    
    delete[] buffer1;
    delete[] buffer2;
  }
  
  {
    int* buffer1 = generate_0(row_midpoint, col_midpoint);
    int* buffer2 = generate_0(row_midpoint, col_midpoint);
    
    _b_matmul(A2,B0,buffer1,row_midpoint,col_midpoint);
    _b_matmul(A3,B2,buffer2, row_midpoint,col_midpoint);
    
    std::vector<std::thread> local_threads;
    for(int tid = 0; tid < num_threads; tid++)
      local_threads.push_back(std::thread(p_mat_add,tid,buffer1,buffer2,C2,row_midpoint/num_threads,col_midpoint));
    
    for(auto& thread: local_threads)
      thread.join();
    
    delete[] buffer1;
    delete[] buffer2;
  }
  
  {
    int* buffer1 = generate_0(row_midpoint, col_midpoint);
    int* buffer2 = generate_0(row_midpoint, col_midpoint);
    
    _b_matmul(A2,B1,buffer1,row_midpoint,col_midpoint);
    _b_matmul(A3,B3,buffer2, row_midpoint,col_midpoint);
    
    std::vector<std::thread> local_threads;
    for(int tid = 0; tid < num_threads; tid++)
      local_threads.push_back(std::thread(p_mat_add,tid,buffer1,buffer2,C3,row_midpoint/num_threads,col_midpoint));
    
    for(auto& thread: local_threads)
      thread.join();
        
    delete[] buffer1;
    delete[] buffer2;
  }
  
  {
    
    // Free Submatrices
    delete[] A0;
    delete[] A1;
    delete[] A2;
    delete[] A3;
    delete[] B0;
    delete[] B1;
    delete[] B2;
    delete[] B3;
//
  }
  // TODO: make use of 16 threads
  
  /* PLUG ON ACTUAL RESULT MATRIX */
  {
    std::thread t0(_b_plug_C0,C0,mat3,row_midpoint,col_midpoint);
    std::thread t1(_b_plug_C1,C1,mat3,row_midpoint,col_midpoint);
    std::thread t2(_b_plug_C2,C2,mat3,row_midpoint,col_midpoint);
    std::thread t3(_b_plug_C3,C3,mat3,row_midpoint,col_midpoint);
    
    t0.join();
    t1.join();
    t2.join();
    t3.join();
  }
  
  auto end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "Blocked Serial? took: " << diff.count() << " sec" << std::endl;
}

