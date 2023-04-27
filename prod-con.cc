// PRODUCER CONSUMER IN C++
#include <thread>
#include <iostream>
#include <queue>
#include <mutex>

#include <condition_variable>

std::condition_variable cond;
std::mutex m;
std::queue<int> shared_queue; 
const int N = 5;

void produce(){
    for(int i =0; i<N; i++){
        m.lock();
        std::cout<<"I produce " << i << std::endl;
        shared_queue.push(i);
        m.unlock();

// sleep for 1 second
std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void busy_consume(){
    for(int i=0; i<N; i++){
        while(shared_queue.empty()){
            m.unlock(); // to let the producer put something
            m.lock(); // lock so that we can then check it
        }
        std::cout << "i read " << shared_queue.front() << std::endl;
        shared_queue.pop();
    }
}

void sleep_consume(){
    for(int i = 0; i < N; i++){
        while(shared_queue.empty()){
            m.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            m.lock();
        }
        std::cout << "I read: " << shared_queue.front() << std::endl;
        shared_queue.pop();
    }
}

void cond_produce(){
    for (int i=0; i<N; i++){
        std::unique_lock<std::mutex> lock(m);
        std::cout << "I produce " << i << std::endl;
        shared_queue.push(i);
        cond.notify_one();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void cond_consume(){
    for(int i = 0; i < N; i++){
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return !shared_queue.empty();});
        
        std::cout <<"I read: " << shared_queue.front() << std::endl;
        shared_queue.pop();
        lock.unlock();
    }

}

int main(){
    std::thread t1(cond_produce);
    std::thread t2(cond_consume);

    t1.join();
    t2.join();

    return 0;

}