//
// Created by Sungjun Park on 2022/12/11.
//

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include "Event.hpp"
#include <thread>
#include <queue>
#include <vector>
#include <condition_variable>

namespace WS
{
  class ThreadPool
  {
  private:
      const size_t NUM_THREADS;
      bool stop;
      std::vector<std::thread> m_threads;
      std::queue<Event> m_jobQueue;
      std::vector<std::queue<Event>* > m_threadJobQueues;
      std::mutex m_jobQueueMutex;
      std::condition_variable m_cvJobQueue;
  public:
      void work(size_t THREAD_NO);
      void enqueueJob(const Event& ev);
      explicit ThreadPool(size_t numThreads);
      ~ThreadPool();
  };
}



#endif //THREADPOOL_HPP