//#include "matrix.h"

void A0_B0(int* mat1, int* mat2, int* mat3, int r, int c){
  int start_row = tid*r;
  int end_row = (tid+1)*r;
  int start_col = tid*c;
  int end_col = (tid+1)*c;
  
  // A0 * B0
  for(int i = start_row; i < end_row; i++){
    for(int j = start_col; j < end_row; j++){
      for(int k = start_row; k < end_row; k++){
        // move nothing
        mat3[i*r + j] += mat1[i*c*2 + k] * mat2[k*r*2 + j];
      }
    }
  }
}

void A0_B1(int* mat1, int* mat2, int* mat3, int r, int c){
  int start_row = 0;
  int end_row = r; // r2
  int start_col = 0;
  int end_col = c; //c2
  
  // A0 * B0
  for(int i = start_row; i < end_row; i++){
    for(int j = start_col; j < end_row; j++){
      for(int k = start_row; k < end_row; k++){
        
        mat3[i*r + j] += mat1[i*c*2 + k] * mat2[k*r*2 + j+c];
      }
    }
  }
}

void A1_B2(int* mat1, int* mat2, int* mat3, int r, int c){
  int start_row = 0;
  int end_row = r;
  int start_col = 0;
  int end_col = c;
  
  // A0 * B0
  for(int i = start_row; i < end_row; i++){
    for(int j = start_col; j < end_row; j++){
      for(int k = start_row; k < end_row; k++){
        mat3[i*r + j] += mat1[i*c*2 + k+r] * mat2[(k+r)*r*2 + j];
      }
    }
  }
}

void A1_B3(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A's column, move B's column and row
        mat3[i*r + j] += mat1[i*c*2 + k+c] * mat2[(k+r)*r*2 + j+c];
      }
    }
  }
}

void A2_B0(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A row,  none on b
        mat3[i*r + j] += mat1[(i+r)*c*2 + k] * mat2[k*r*2 + j];
      }
    }
  }
}

void A2_B1(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A: row, move B: col
        mat3[i*r + j] += mat1[(i+r)*c*2 + k] * mat2[k*r*2 + j+c];
      }
    }
  }
}

void A3_B2(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A: col row, move B: row
        mat3[i*r + j] += mat1[(i+r)*c*2 + k+c] * mat2[(k+r)*r*2 + j];
      }
    }
  }
}

void A3_B3(int* mat1, int* mat2, int* mat3, int r, int c){
  for(int i = 0; i < r; i++){
    for(int j = 0; j < c; j++){
      for(int k = 0; k < r; k++){
        // move A: row, col; move B: row col
        mat3[i*r + j] += mat1[(i+r)*c*2 + k+c] * mat2[(k+r)*r*2 + j+c];
      }
    }
  }
}

void _C0(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(A0_B0, mat1,mat2,mat3,r,c);
  std::thread t1(A1_B2, mat1,mat2,mat3,r,c);
  t0.join();
  t1.join();
}

void _C1(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(A0_B1, mat1,mat2,mat3,r,c);
  std::thread t1(A1_B3, mat1,mat2,mat3,r,c);
  t0.join();
  t1.join();
}

void _C2(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(A2_B0, mat1,mat2,mat3,r,c);
  std::thread t1(A3_B2, mat1,mat2,mat3,r,c);
  t0.join();
  t1.join();
}

void _C3(int* mat1, int* mat2, int* mat3, int r, int c){
  std::thread t0(A2_B1, mat1,mat2,mat3,r,c);
  std::thread t1(A3_B3, mat1,mat2,mat3,r,c);
  t0.join();
  t1.join();
}


void combine(int* destination, int* m0, int* m1, int* m2, int* m3,  int r, int c){
  // C0:
  for(int i = 0;  i < r; i++)
    for(int j = 0; j < c; j++)
      destination[i*r*2 + j] = m0[i*r + j];
    
  // C1:
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      destination[i*r*2 + j+c] = target[i*r + j];
   
  
  // C2
  for(int i = 0;  i < r; i++)
    for(int j = 0; j < c; j++)
      destination[(i+r)*r*2 + j] = m0[i*r + j];
  
  // C3:
  for(int i = 0;  i < r; i++)
    for(int j = 0; j < c; j++)
      destination[(i+r)*r*2 + j+c] = m0[i*r + j];
}


void b_matrix(int* mat1, int* mat2, int* mat3, int r, int c){
  int r2 = r / 2;
  int c2 = c / 2;
  
  
  
}
