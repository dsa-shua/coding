#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>

// the use of condition variable to make our own barrier
std::mutex global_lock;
std::condition_variable cond;
int done = 0;
int num_threads = 8;

void barrier(int tid){
  std::unique_lock<std::mutex> lock(global_lock);
  std::cout << "TID: " << tid << " has entered." << std::endl;
  done++;
  if (done == num_threads)
    cond.notify_all();
  cond.wait(lock, []{return (done == num_threads);});

  std::cout << "TID: " << tid << " passed the barrier!" << std::endl;
}

int main(void){
  std::vector<std::thread> threads;
  for(int i = 0; i < num_threads; i++){
    threads.push_back(std::thread(barrier, i));
  }
  for(auto& thread: threads){
    thread.join();
  }
  std::cout << "MAIN THREAD" << std::endl;
  return 0;
}
