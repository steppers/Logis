#include "semaphore.h"

Semaphore::Semaphore() noexcept
:	m_count(0)
{}
 
void Semaphore::Signal() noexcept
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		++m_count;
	}
	m_cv.notify_one();
}

void Semaphore::Wait() noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock, [&]() { return m_count != 0; });
	--m_count;
}

FastSemaphore::FastSemaphore() noexcept
:	m_count(0)
{}
 
void FastSemaphore::Signal()
{
	std::atomic_thread_fence(std::memory_order_release);
	int count = m_count.fetch_add(1, std::memory_order_relaxed);
	if (count < 0)
		m_semaphore.Signal();
}

void FastSemaphore::Wait()
{
	int count = m_count.fetch_sub(1, std::memory_order_relaxed);
	if (count < 1)
		m_semaphore.Wait();
	std::atomic_thread_fence(std::memory_order_acquire);
}
