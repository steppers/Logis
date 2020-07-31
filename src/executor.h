#ifndef _EXECUTOR_H_
#define _EXECUTOR_H_

#include <atomic>
#include <thread>
#include <functional>

#include "semaphore.h"
#include "stealing_queue.h"
#include "pin.h"

struct TriggerJob {
	PinObserver*	context;
	Pin*			pin;
};

class TriggerExecutor {
public:
	TriggerExecutor();

	void			add_job(PinObserver* ctx, Pin* pin);
	bool 			do_job(void* thread_ctx);
	void			run();

private:
	std::vector<StealingQueue<TriggerJob>*>	m_queues;
	std::atomic<uint32_t>					m_next_queue;
	std::thread**							m_threads;

	Semaphore								m_wait_sem;
	Semaphore								m_run_wait_sem;
	std::atomic<uint64_t>					m_current;

	uint64_t								m_num_added_not_running;
	bool									m_paused;
};
extern TriggerExecutor gExecutor;

#endif // _EXECUTOR_H_