//#include "matrix.h"
#include <mutex>

void A0_B0(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move nothing
        mat3[i*r*2 + j] += mat1[i*c*2 + k] * mat2[k*r*2 + j];
      }
    }
  }
}

void A0_B1(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        mat3[i*r*2 + j+c] += mat1[i*c*2 + k] * mat2[k*r*2 + j+c];
      }
    }
  }
}

void A1_B2(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        mat3[i*r*2 + j] += mat1[i*c*2 + k+r] * mat2[(k+r)*r*2 + j];
      }
    }
  }
}

void A1_B3(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A's column, move B's column and row
        mat3[i*r*2 + j+c] += mat1[i*c*2 + k+c] * mat2[(k+r)*r*2 + j+c];
      }
    }
  }
}

void A2_B0(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A row,  none on b
        mat3[(i+r)*r*2 + j] += mat1[(i+r)*c*2 + k] * mat2[k*r*2 + j];
      }
    }
  }
}

void A2_B1(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A: row, move B: col
        mat3[(i+r)*r*2 + j+c] += mat1[(i+r)*c*2 + k] * mat2[k*r*2 + j+c];
      }
    }
  }
}

void A3_B2(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
//  printf("A3_B2 | TID: %d | sr: %d | er: %d\n", tid, start_row, end_row);
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A: col row, move B: row
        mat3[(i+r)*r*2 + j] += mat1[(i+r)*c*2 + k+c] * mat2[(k+r)*r*2 + j];
      }
    }
  }
}

void A3_B3(int* mat1, int* mat2, int* mat3, int r, int c, int tid){
  int start_row = 0 + tid*(r/2);
  int end_row = r/2 + tid*(r/2);
  
  for(int i = start_row; i < end_row; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A: row, col; move B: row col
        mat3[(i+r)*r*2 + j+c] += mat1[(i+r)*c*2 + k+c] * mat2[(k+r)*r*2 + j+c];
      }
    }
  }
}

void p0(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(A0_B0, mat1,mat2,mat3,r,c,0);
  std::thread t1(A0_B1, mat1,mat2,mat3,r,c,0);
  std::thread t2(A2_B0, mat1,mat2,mat3,r,c,0);
  std::thread t3(A2_B1, mat1,mat2,mat3,r,c,0);
  std::thread t4(A1_B2, mat1,mat2,mat3,r,c,1);
  std::thread t5(A1_B3, mat1,mat2,mat3,r,c,1);
  std::thread t6(A3_B2, mat1,mat2,mat3,r,c,1);
  std::thread t7(A3_B3, mat1,mat2,mat3,r,c,1);
  
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
}

void p1(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(A0_B0, mat1,mat2,mat3,r,c,1);
  std::thread t1(A0_B1, mat1,mat2,mat3,r,c,1);
  std::thread t2(A2_B0, mat1,mat2,mat3,r,c,1);
  std::thread t3(A2_B1, mat1,mat2,mat3,r,c,1);
  std::thread t4(A1_B2, mat1,mat2,mat3,r,c,0);
  std::thread t5(A1_B3, mat1,mat2,mat3,r,c,0);
  std::thread t6(A3_B2, mat1,mat2,mat3,r,c,0);
  std::thread t7(A3_B3, mat1,mat2,mat3,r,c,0);
  
  t0.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
}

void b_matrix(int* mat1, int* mat2, int* mat3, int r, int c){
//  std::cout << "BLOCKED MATRIX" << std::endl;
  std::chrono::duration<double> diff;
  auto start_time = std::chrono::steady_clock::now();
  int r2 = r / 2;
  int c2 = c / 2;
  
  p0(mat1, mat2, mat3, r2,c2);
  p1(mat1, mat2, mat3, r2,c2);
  
  auto end_time = std::chrono::steady_clock::now();
  diff = end_time - start_time;
  std::cout << "B Standard Matrix took: " << diff.count() << " sec" << std::endl;
}
