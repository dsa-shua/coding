#include "matrix.h"

void test1(void){
  dataSheet();
  printMatrix(A);
  printMatrix(B);
  printMatrix(reference);
  serial_matmul();
  printMatrix(C);
  sanity();
  serial_transpose();
  printMatrix(C);
}

int main(void){
  dataSheet();
  parallel_row();
  sanity();
  return 0;
}
