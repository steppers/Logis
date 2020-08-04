#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include <atomic>
#include <thread>
#include <functional>
#include <deque>

#include "semaphore.h"
#include "stealing_stack.h"

#define JOB_BUCKET_SIZE 16

class Pin;
class PinObserver;

struct TriggerJob {
	PinObserver*	context;
	Pin*			pin;
};

struct JobBucket {
	TriggerJob		jobs[JOB_BUCKET_SIZE];
	uint8_t			num_jobs;
};

class SingleThreadExecutor {
public:
	void			add_job(TriggerJob job);
	bool 			do_job(void* thread_ctx);
	void			run();

private:
	std::deque<TriggerJob>					m_queue;
};

class MultiThreadExecutor {
public:
	MultiThreadExecutor();

	void			add_job(TriggerJob job);
	bool 			do_job();
	void			run();

private:
	uint32_t								m_num_threads;
	std::thread**							m_threads;
	StealingStack<JobBucket>*				m_queues;

	FastSemaphore							m_job_sem;

	std::atomic<uint64_t>					m_jobs; // shared jobs
	bool									m_running;

	uint64_t								m_jobs_added;

	bool									do_thread_jobs();
};

extern MultiThreadExecutor gExecutor;

#endif // _EXECUTOR_H_