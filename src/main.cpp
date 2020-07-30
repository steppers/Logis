#include <stdint.h>
#include <vector>
#include <deque>
#include <iostream>
#include <mutex>

class Pin;
class PinObserver {
public:
	virtual void on_trigger(Pin* p) = 0;

private:
	std::mutex m_trigger_mut;

	void on_trigger_internal(Pin* p) {
		m_trigger_mut.lock();
		on_trigger(p);
		m_trigger_mut.unlock();
	}

	friend class TriggerExecutor;
};

struct TriggerJob {
	PinObserver*	context;
	Pin*			pin;
};

class TriggerExecutor {
public:
	void add_job(PinObserver* ctx, Pin* pin) {
		m_jobs.push_back({ctx, pin});
	}

	void run() {
		while(!m_jobs.empty()) {
			auto job = m_jobs.front();
			m_jobs.pop_front();
			job.context->on_trigger_internal(job.pin);
		}
	}

private:
	std::deque<TriggerJob> m_jobs;
};
static TriggerExecutor gExecutor;

class Pin {
private:
	uint64_t				  m_state;
	std::vector<PinObserver*> m_observers;

public:
	Pin() : m_state(0) {}

	void set_state(uint64_t s) {
		m_state = s;
		trigger_observers();
	}

	uint64_t get_state() const {
		return m_state;
	}

public:
	void register_observer(PinObserver* o) {
		m_observers.push_back(o);
	}

	void trigger_observers() {
		for(auto o : m_observers) {
			gExecutor.add_job(o, this);
		}
	}
};

class ANDGate : PinObserver {
private:
	Pin* in_a;
	Pin* in_b;
	Pin out;

	void on_trigger(Pin* o)
	{
		out.set_state(in_a->get_state() & in_b->get_state());
	}

public:
	void set_inputs(Pin& a, Pin& b)
	{
		in_a = &a;
		in_b = &b;
		a.register_observer(this);
		b.register_observer(this);
		gExecutor.add_job(this, in_a);
	}

	Pin& get_out()
	{
		return out;
	}
};



class ORGate : PinObserver {
private:
	Pin* in_a;
	Pin* in_b;
	Pin out;

	void on_trigger(Pin* o)
	{
		out.set_state(in_a->get_state() | in_b->get_state());
	}

public:
	void set_inputs(Pin& a, Pin& b)
	{
		in_a = &a;
		in_b = &b;
		a.register_observer(this);
		b.register_observer(this);
		gExecutor.add_job(this, in_a);
	}

	Pin& get_out()
	{
		return out;
	}
};

class Adder : PinObserver {
private:
	Pin* in_a;
	Pin* in_b;
	Pin out;

	void on_trigger(Pin* o)
	{
		out.set_state(in_a->get_state() + in_b->get_state());
	}

public:
	void set_inputs(Pin& a, Pin& b)
	{
		in_a = &a;
		in_b = &b;
		a.register_observer(this);
		b.register_observer(this);
		gExecutor.add_job(this, in_a);
	}

	Pin& get_out()
	{
		return out;
	}
};

#define PRINT_NET(name, pin) \
	std::cout << "[PIN] " name ": " << pin.get_state() << std::endl;

int main()
{
	
	Pin a, b, c, d;

	ANDGate gateA;
	ORGate gateB;
	Adder gateC;

	gateA.set_inputs(a, b);
	gateB.set_inputs(c, d);
	gateC.set_inputs(gateA.get_out(), gateB.get_out());

	const Pin& out = gateC.get_out();

	a.set_state(0b0110);
	b.set_state(0b0011);

	c.set_state(0b1000);
	d.set_state(0b0011);

	// Initial Run
	gExecutor.run();

	PRINT_NET("a", a);
	PRINT_NET("b", b);
	PRINT_NET("c", c);
	PRINT_NET("d", d);
	PRINT_NET("out", out);
}
