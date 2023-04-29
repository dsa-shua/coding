#include "matrix.h"

/*
 TODO: :
 Make a matrix generator for arbitrary square matrices
 1) Check columnw-wise and row-wise parallelizations
 2) Do transposed
 3) Do blocked matrix
 */

void run(void){
  //  int r = 1024;
  //  int c = 1024;
  //  int* mat1 = generate_matrix(r,c);
  //  int* mat2 = generate_matrix(r,c);
  //  int* mat3 = generate_0(r,c);
  //  int* ref = generate_0(r,c);
  //  serial_matmul(mat1, mat2, ref, r,c);
  //  parallel_row(mat1,mat2,mat3,r,c);
  //  sanity_check(ref, mat3);
}

int main(void){
  int r = 8;
  int c = 8;
  int* mat = generate_matrix(r,c);
  int* transposed = generate_0(r,c);
  printMatrix(mat,r,c);
  serial_transpose(mat, transposed, r,c);
  printMatrix(transposed,c,r);
  
  return 0;
}
