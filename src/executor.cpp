#include "executor.h"

TriggerExecutor gExecutor;

struct ThreadContext {
	uint32_t		id;
};

void trigger_thread(ThreadContext ctx) {
	while(true) {
		if (gExecutor.do_job(&ctx) == false) {
			break;
		}
	}
}

TriggerExecutor::TriggerExecutor()
:	m_next_queue(0)
,	m_paused(true)
,	m_current(0)
{
	uint32_t threads = std::thread::hardware_concurrency();
	m_threads = new std::thread*[threads];

	for (uint32_t i = 0; i < threads; ++i) {
		m_queues.push_back(new StealingQueue<TriggerJob>());
		ThreadContext ctx = { i };
		m_threads[i] = new std::thread(trigger_thread, ctx);
	}
}

void TriggerExecutor::add_job(PinObserver* ctx, Pin* pin) {
	m_queues[m_next_queue++ % m_queues.size()]->push({ctx, pin});
	++m_current;
	if (m_paused) {
		++m_num_added_not_running;
	} else {
		m_wait_sem.Signal();
	}
}

bool TriggerExecutor::do_job(void* thread_ctx) {
	ThreadContext* thread = (ThreadContext*)thread_ctx;
	TriggerJob job;

	// Wait for signal
	m_wait_sem.Wait();

	while (true) {
		uint32_t num_queues = m_queues.size();
		for (size_t i = 0; i < num_queues; i++) {
			if (m_queues[(thread->id + i) % num_queues]->pop(job)) {
				job.context->on_trigger_lock(job.pin);

				// Signal to the run() function if all jobs are completed
				if(m_current.fetch_sub(1) == 1)
				{
					m_run_wait_sem.Signal();
				}
				return true;
			}
		}
	}
}

void TriggerExecutor::run() {
	if (m_num_added_not_running == 0) {
		return;	
	}

	m_paused = false;

	for (uint64_t i = 0; i < m_num_added_not_running; ++i) {
		m_wait_sem.Signal();
	}
	m_num_added_not_running = 0;

	m_run_wait_sem.Wait();

	m_paused = true;
}