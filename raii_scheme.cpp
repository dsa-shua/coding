#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

std::mutex global_lock;
int global_data = 0;
int num_threads = 8;

// RAII: Resource Allocation is Initialization
class LOCK{
private:
  std::mutex& m;
public:
    LOCK(std::mutex& m_) : m(m_) {// allocation
    m.lock(); // is initialization
  }
  ~LOCK(){ //on destruction, automatically unlcoks
    m.unlock();
  }
};


void process(void){
  LOCK local_lock(global_lock);
  for(int i = 0; i < 123456789; i++)
    global_data++;
}


int main(void){
  
  std::vector<std::thread> threads;
  
  for(int i = 0; i <num_threads;i++){
    threads.push_back(std::thread(process));
  }
  for(auto& thread: threads){
    thread.join();
  }
  
  std::cout << "Global data: " << global_data << std::endl;
  return 0;
}
