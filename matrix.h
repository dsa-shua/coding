#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

const int n = 8;
const int m = 8;
const int size = 64;
int A[]= {
  1,3,4,4,5,1,5,2,
  1,1,3,3,5,5,3,5,
  4,3,2,1,5,2,5,1,
  2,5,5,5,4,2,2,5,
  3,5,1,4,3,2,3,3,
  4,2,1,4,1,3,1,4,
  3,2,1,1,2,4,1,5,
  3,3,1,5,1,1,4,2
};
int B[] = {
  3,3,1,2,1,1,4,1,
  3,2,4,5,1,2,5,1,
  4,1,4,1,2,1,3,4,
  5,2,3,2,3,3,1,4,
  1,5,1,1,1,4,5,3,
  1,4,3,3,5,4,2,4,
  2,3,2,1,2,1,2,4,
  1,5,1,5,2,1,4,4
};
int reference[] = {
  66,75,61,52,48,54,80,83,
  54,93,57,64,63,63,82,93,
  52,75,49,48,41,49,81,66,
  81,90,76,81,60,63,101,95,
  62,75,57,67,47,52,81,69,
  52,65,44,58,46,42,62,61,
  37,70,39,59,44,41,66,59,
  59,57,48,50,41,39,58,61
};
int C[] ={
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};
const int num_threads = 4;
std::mutex global_lock;
int items_per_thread = size / num_threads;
int row_per_thread = n / num_threads;
int col_per_thread = m / num_threads;

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

void sanity(void){
  for(int i = 0; i < n * m; i++)
    if(C[i] != reference[i])
      std::cout << "Error at index: " << i << "C: " << C[i] << " | Ref: " << reference[i] << std::endl;
}

void printMatrix(int* mat){
  for(int i = 0; i < n * m; i++){
    if ((i % n) == 0) printf("\n");
    printf("%3d ", mat[i]);
  }
  printf("\n");
}

void serial_matmul(void){
  std::cout << "Serial Matrix Multiplication" << std::endl;
  for(int i = 0; i < n; i++)
    for(int j = 0; j < m; j++)
      for(int k = 0; k < n; k++){
        C[i*n + j] += A[i*m + k] * B[k*n + j];
      }
}

void serial_transpose(void){
  // transposing A
  for(int i = 0; i < n; i++)
    for(int j = 0; j < m; j++)
      for(int k = 0; k < n; k++)
        C[i*m + j] = A[j*m + i];
}

void row_wise(int tid){
  std::unique_lock<std::mutex> lock(global_lock);
  int start_row = tid*row_per_thread; // starting row
  int end_row = start_row + row_per_thread;
  int start = tid*items_per_thread;
  int end = start + items_per_thread;

  printf("TID: %d | start_row: %2d | end_row: %2d | start_ind: %2d | end_ind: %d\n", tid, start_row, end_row, start, end);

  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < m; j++){
      printf("[%d] row: %d, col: %d\n", tid, i,j);
      for(int k = 0; k < n; k++){
        C[i*n + j] += A[i*m + k] * B[k*n+ j];
      }
    }
  }

} // row_wise end

void parallel_row(void){
  std::vector<std::thread> threads;
  for(int tid = 0; tid < num_threads; tid++){
    // from here we choose what we run
    threads.push_back(std::thread(row_wise, tid));
  }
  for(auto& thread: threads){
    thread.join();
  }
}




