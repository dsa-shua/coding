#include <thread>
#include <iostream>


int global_data = 0;
int num_threads = 8;

void worker(int tid){
  std::cout << "TID: " << tid << std::endl;
}

int main(void){
  std::vector<std::thread> threads;
  
  for(int tid = 0; tid < num_threads; tid++){
    threads.push_back(std::thread(worker,tid));
  }
  
  for(auto& thread: threads){
    thread.join();
  }
  
  return 0;
}
