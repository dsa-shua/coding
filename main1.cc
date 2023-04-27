#include <iostream>
#include <thread>
#include <vector>

int *a, *b, *k, *c;
const int N;
int NT; // num threads

void mac(int tid, int num_threads){
    for (int i = 0; i < N/num_threads; i++){
        int idx = tid*(N/num_threads) + i;
        c[idx] = k[idx] * a[idx];
        c[idx] += k[idx] * b[idx];
    }
    return;
}

int main(int argc, char* argv[]){
    std::vector<std::thread> threads;
    int a[N], b[N], k[N], c[N];
    
    // initialize the arrays
    for (int i = 0; i < N; i++){
        a[i] = i;
        b[i] = 1000*i;
        k[i] = 10;
        c[i] = 0;
    }

    for (int t=0; t <NT; t++){
        threads.push_back(std::thread(mac, t ,NT));
    }

    for (auto& thread: threads){
        thread.join();
    }

    return 0;
}