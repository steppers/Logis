#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <mutex>
#include <condition_variable>

class Semaphore {
public:
	Semaphore();

	void Signal();
	void Wait();

private:
	uint64_t					m_val;

	std::mutex					m_mut;
	std::condition_variable		m_cv;
};

#endif // _SEMAPHORE_H_