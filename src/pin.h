#ifndef _PIN_H_
#define _PIN_H_

#include <stdint.h>
#include <vector>
#include <mutex>

class Pin;

class PinObserver {
public:
	virtual void on_trigger(Pin* p) = 0;
	void on_trigger_lock(Pin* p);

private:
	std::mutex m_trigger_mut;
};

class Pin {
protected:
	uint64_t				  m_state;
	bool					  m_floating;

private:
	std::vector<PinObserver*> m_observers;

public:
	Pin();

	void set_state(uint64_t s);
	uint64_t get_state() const;

	void set_floating(bool enable);

public:
	void register_observer(PinObserver* o);
	void trigger_observers();
};

#endif // _PIN_H_