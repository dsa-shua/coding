#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

const int num_threads = 4; // equal to num cols
std::vector<std::thread> threads;
const int n = 4;
const int m = 4;


int A[] = {9,1,7,2,
          8,8,7,7,
          9,9,4,1,
          6,7,6,3};

int B[] = {1,3,4,5,
          1,9,7,5,
          8,1,8,6,
          7,4,2,7};

int C[] = {0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0};

int D[] = {0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0};

int E[] = {0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0};

void sequential(void){
  for(int i = 0; i < n; i++)
    for(int j = 0; j < m; j++)
      for(int k = 0; k < n; k++) // kth item on row(i)
        C[i * n + j] += A[i * m + k] *B[k * n + j];
  //C[<move to row by A> <offset by which column in B>]
  // A[ move by mth row of A, offset to kth item ]
  // B[ move to nth row of B, offset by its column ]
      
  for(int i = 0; i < n * m; i++){
    if((i % n) == 0){
      printf("\n");
    }
    printf(" %3d ", C[i]);
  }
  printf("\n");
}


void work(int i){
  for(int j = 0; j < m; j++){
    for (int k = 0; k < n; k++){
      D[i * n + j] += A[i*m + k] * B[k * n + j];
    }
  }
}



void parallel(void){
  std::thread t0(work,0);
  std::thread t1(work,1);
  std::thread t2(work,2);
  std::thread t3(work,3);
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  for(int i = 0; i < n * m; i++){
    if((i % n) == 0){
      printf("\n");
    }
    printf(" %3d ", D[i]);
  }
  printf("\n");
}

void work_col(int i, int j){
  for (int k = 0; k < n; k++){
    E[i * n + j] += A[i*m + k] * B[k * n + j];
  }
  
}

void col_parallel(void){
  for(int i = 0; i < n; i++){
    std::thread t0(work_col,i,0);
    std::thread t1(work_col,i,1);
    std::thread t2(work_col,i,2);
    std::thread t3(work_col,i,3);
    
    t0.join();
    t1.join();
    t2.join();
    t3.join();
  }
  
  for(int i = 0; i < n * m; i++){
    if((i % n) == 0){
      printf("\n");
    }
    printf(" %3d ", E[i]);
  }
  printf("\n");
}

int main(void){
  std::chrono::duration<double> diff;
  auto start = std::chrono::steady_clock::now();
  sequential();
  auto end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "Sequential Took: " << diff.count() << std::endl;
  start = std::chrono::steady_clock::now();
  parallel();
  end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "Parallel Took: " << diff.count() << std::endl;
  start = std::chrono::steady_clock::now();
  col_parallel();
  end = std::chrono::steady_clock::now();
  diff = end - start;
  std::cout << "Col Parallel Took: " << diff.count() << std::endl;
  return 0;
}
