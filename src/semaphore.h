#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>
#include <atomic>

class Semaphore
{
public:
    Semaphore() noexcept;
 
    void Signal() noexcept; 
    void Wait() noexcept;
 
private:
    int m_count;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};
 
class FastSemaphore
{
public:
    FastSemaphore() noexcept;
 
    void Signal();
    void Wait();
 
private:
    std::atomic<int> m_count;
    Semaphore m_semaphore;
};

#endif // _SEMAPHORE_H_