#include "pin.h"

Pin::Pin()
:	m_state(0)
,	m_floating(true)
{}

void Pin::set_state(uint64_t s) {
	m_state.store(s, std::memory_order_relaxed);
	// m_floating = false;
	trigger_observers();
}

uint64_t Pin::get_state() const {
	return m_state.load(std::memory_order_relaxed);
}

void Pin::set_floating(bool enable) {
	m_floating = enable;
	trigger_observers();
}

void Pin::register_observer(PinObserver* o) {
	m_observers.push_back({o, this});
} 

void Pin::trigger_observers() {
	for(auto o : m_observers) {
		gExecutor.add_job(o);
	}
}

void PinObserver::on_trigger_lock(Pin* p) {
	m_trigger_mut.lock();
	on_trigger(p);
	m_trigger_mut.unlock();
}