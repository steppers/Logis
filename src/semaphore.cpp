#include "semaphore.h"

Semaphore::Semaphore()
:	m_val(0)
{}

void Semaphore::Signal() {
	std::lock_guard<std::mutex> lock(m_mut);
	++m_val;
	m_cv.notify_one();
}

void Semaphore::Wait() {
	std::unique_lock<std::mutex> lock(m_mut);
	while(m_val == 0) {
		m_cv.wait(lock);
	}
	--m_val;
}