#include "matrix.h"
#include "blkd_matmul.h"
#include "block_transpose.h"

int r = n;
int c = m;

int main(void){
  srand(time(NULL));
  
  
  int* matrix1 = generate_matrix(r,c);
  int* matrix2 = generate_matrix(r,c);
  
  {
//    int* serial = generate_0(r,c);
    int* block = generate_0(r,c);
    int* trans = generate_0(r,c);
    int* bmat = generate_0(r,c);
    int* bmat1= generate_0(r,c);
//    serial_matmul(matrix1, matrix2, serial, r,c); clear();
    blocked_transpose(matrix1, matrix2, block, r,c);
    parallel_transpose(matrix1, matrix2, trans, r,c);
    b_matrix(matrix1, matrix2, bmat, r,c);
    blocked_matrix(matrix1, matrix2, bmat1, r,c);
//    printf("Blocked: "); sanity_check(serial, block,r,c);
    
  }
  return 0;
}
