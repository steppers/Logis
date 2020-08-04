#include "executor.h"

#include "pin.h"

MultiThreadExecutor gExecutor;

//////////////////////////////////////////////////////////////////////////////////
// Single threaded
//////////////////////////////////////////////////////////////////////////////////

void SingleThreadExecutor::add_job(TriggerJob job) {
	m_queue.push_back(job);
}

void SingleThreadExecutor::run() {
	while(!m_queue.empty()) {
		m_queue.front().context->on_trigger(m_queue.front().pin);
		m_queue.pop_front();
	}
}

bool SingleThreadExecutor::do_job(void* thread_ctx) { return false; }


//////////////////////////////////////////////////////////////////////////////////
// Multithreaded
//////////////////////////////////////////////////////////////////////////////////
thread_local uint32_t	thread_id;
thread_local JobBucket	thread_bucket;

void thread_func(uint32_t id, MultiThreadExecutor* exec) {
	thread_id = id;
	thread_bucket.num_jobs = 0;

	while (true) {
		if (!exec->do_job()) {
			break;
		}
	}
}

MultiThreadExecutor::MultiThreadExecutor()
:	m_num_threads(std::thread::hardware_concurrency())
,	m_running(false)
{
	thread_id = 0;
	thread_bucket.num_jobs = 0;

	m_threads = new std::thread*[m_num_threads];
	m_queues = new StealingStack<JobBucket>[m_num_threads + 1];

	for(uint32_t i = 0; i < m_num_threads; ++i) {
		m_threads[i] = new std::thread(thread_func, i + 1, this);
	}
}

void MultiThreadExecutor::add_job(TriggerJob job) {
	// Add job to thread bucket
	thread_bucket.jobs[thread_bucket.num_jobs++] = job;

	// If the bucket is full, add it to the shared list
	if(thread_bucket.num_jobs == JOB_BUCKET_SIZE) {
		m_jobs.fetch_add(1, std::memory_order_relaxed);
		m_queues[thread_id].push(thread_bucket);
		thread_bucket.num_jobs = 0;

		if (!m_running) {
			++m_jobs_added;
		} else {
			m_job_sem.Signal();
		}
	}
}

bool MultiThreadExecutor::do_job() {
	const uint32_t num_queues = m_num_threads + 1;

	JobBucket bucket;

	if(do_thread_jobs())
		return true;

	m_job_sem.Wait();

	if(do_thread_jobs())
		return true;

	while (true) {
		for (size_t i = 0; i < num_queues; i++) {
			if (m_queues[(thread_id + i) % num_queues].pop(bucket)) {
				for(uint8_t i = 0; i < bucket.num_jobs; ++i) {
					bucket.jobs[i].context->on_trigger(bucket.jobs[i].pin);
				}

				m_jobs.fetch_sub(1, std::memory_order_relaxed);
				return true;
			}
		}
	}

	return false;
}

bool MultiThreadExecutor::do_thread_jobs() {
	if(thread_bucket.num_jobs > 0) {
		JobBucket bucket = thread_bucket;
		thread_bucket.num_jobs = 0;

		for(uint8_t i = 0; i < bucket.num_jobs; ++i) {
			TriggerJob& job = bucket.jobs[i];
			job.context->on_trigger(job.pin);
		}
		return true;
	}

	return false;
}

void MultiThreadExecutor::run() {
	m_running = true;

	// Signal for all added buckets
	for (uint64_t i = 0; i < m_jobs_added; ++i) {
		m_job_sem.Signal();
	}
	m_jobs_added = 0;

	// Do jobs in the main thread's bucket
	while(do_thread_jobs() || m_jobs.load(std::memory_order_relaxed)) {}

	m_running = false;
}
