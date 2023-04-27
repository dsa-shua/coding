#include<thread>
#include<iostream>
#include <vector>
#include <mutex>

std::mutex global_lock;

int count = 0;
void worker(int* input, int start, int size, int* output){

    for(int i=0;i <size; i++){
        if (input[start+i]==0){
            // global_lock.lock();
            std::lock_guard<std::mutex> guard(global_lock);
            (*output)++;
            // global_lock.unlock();
        }
    }
}

int main(int argc, char* argv[]){
    if (argc != 3){
        std::cout << "Usage :" << argv[0] << "<N> <NT>" << std::endl;
        exit(1);
    }
    const int N = atoi(argv[1]);
    const int NT = atoi(argv[2]);

    int* array = new int[N];

    for (int i = 0; i < N; i++){
        array[i] = 0;
    }

    
    std::vector<std::thread> threads;

   for(int t=0; t<NT;t++){
        int size=N/NT;
        int start = t*size;
        threads.push_back(std::thread(worker, array, start, size, &count));
   }

    for(auto& thread: threads){
        thread.join();
    }

    std::cout << "there are " << count << " zeros" << std::endl;
    return 0;
}