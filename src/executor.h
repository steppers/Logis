#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include <atomic>
#include <thread>
#include <functional>
#include <deque>

#include "semaphore.h"
#include "stealing_stack.h"
#include "pin.h"

#define JOB_BUCKET_SIZE 1

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
	void			add_job(PinObserver* ctx, Pin* pin);
	bool 			do_job(void* thread_ctx);
	void			run();

private:
	std::deque<TriggerJob>					m_queue;
};

class MultiThreadExecutor {
public:
	MultiThreadExecutor();

	void			add_job(PinObserver* ctx, Pin* pin);
	bool 			do_job();
	void			run();

private:
	uint32_t								m_num_threads;
	std::thread**							m_threads;
	StealingStack<JobBucket>*				m_queues;
	JobBucket*								m_thread_buckets;

	FastSemaphore							m_job_sem;
	FastSemaphore							m_run_sem;

	std::atomic<uint64_t>					m_jobs;
	uint64_t								m_jobs_na;
	std::atomic<bool>						m_running;

	uint64_t								m_jobs_added;

	bool									do_thread_job();
};

extern MultiThreadExecutor gExecutor;

#endif // _EXECUTOR_H_