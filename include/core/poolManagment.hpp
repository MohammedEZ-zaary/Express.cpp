#ifndef POOL_MANAGMENT
#define POOL_MANAGMENT
#include "threadPool.hpp"

class PoolManagment {
  public:
      PoolManagment() : pool1(4) , pool2(4) { // Initialize ThreadPool with 3 threads
      }
  
      void addIo(std::function<void()> task) {
          pool1.enqueue(task);
      }
      void addRequestExecution(std::function<void()> task) {
          pool2.enqueue(task);
      }
  
  private:
      ThreadPool pool1; // ThreadPool instance
      ThreadPool pool2; // ThreadPool instance
  };
  

#endif