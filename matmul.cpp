#include "matmul.h"

// matatmul does: (n x m) times (m x n)

void matmul_ref(const int*  const matrixA, const int* const matrixB, int* const matrixC, const int n, const int m){
    for (int i= 0; i < n; i++) // rows of A
        for(int j = 0; j < n; j++) // cols ofB
            for(int k = 0; k < m; k++) // items of row A and col B multipy
                matrixC[i * n + j] += matrixA[i * m + k] * matrixB[k * n + j];
}


void matmul_optimized(const int* const matrixA, const int* const matrixB, int* const matrixC, const int n, const int m){


    // Implement your code
}



    



