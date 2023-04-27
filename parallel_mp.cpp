#include <iostream>
#include <chrono>
#include <omp.h>

const int N = 123456789;
int *a,*b,*k,*c;

int main(void){
  
#pragma omp parallel for
  for (int i = 0; i < 10; i++){
    std::cout << "Hello World!" << std::endl;
  }
  
  
  return 0;
}

int main1(void){
    a = new int[N];
    k = new int[N];
    b = new int[N];
    c = new int[N];

    std::chrono::duration<double> diff;
    {
        auto fill_start = std::chrono::steady_clock::now();
        for(int i = 0; i < N; i++){
            a[i] = i;
            b[i] = 1000*i;
            k[i] = 10;
            c[i] = 0;
        }
        auto fill_end = std::chrono::steady_clock::now();
        diff = fill_end - fill_start;
        std::cout << "Init took: " << diff.count() << " sec" << std::endl;
    }
    
    {
        auto start = std::chrono::steady_clock::now();
        for(int i=0; i <N; i++){
            c[i] = k[i]*a[i];
            c[i] += k[i]*b[i];
        }
        auto end = std::chrono::steady_clock::now();
        diff = end - start;
        std::cout << "Process took: " << diff.count() << " sec" << std::endl;
    }


// OPEN MP
    {
        auto fill_start = std::chrono::steady_clock::now();
        #pragma omp parallel for
        for(int i = 0; i < N; i++){
            a[i] = i;
            b[i] = 1000*i;
            k[i] = 10;
            c[i] = 0;
        }
        auto fill_end = std::chrono::steady_clock::now();
        diff = fill_end - fill_start;
        std::cout << "OPEN MP Init took: " << diff.count() << " sec" << std::endl;
    }

    {
        auto start = std::chrono::steady_clock::now();
        #pragma omp parallel for
        for(int i=0; i <N; i++){
            c[i] = k[i]*a[i];
            c[i] += k[i]*b[i];
        }
        auto end = std::chrono::steady_clock::now();
        diff = end - start;
        std::cout << "OPEN MP Process took: " << diff.count() << " sec" << std::endl;
    }
  
  delete a;
  delete b;
  delete c;
  delete k;
    return 0;
}
