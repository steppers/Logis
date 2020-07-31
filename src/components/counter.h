#ifndef _COUNTER_H_
#define _COUNTER_H_

#include "../pin.h"

class Counter : PinObserver {
private:
	Pin* in_load;
	Pin* in_data;
	Pin* in_count;
	Pin* in_clock;
	Pin* in_clear;
	Pin out;

	void on_trigger(Pin* o);

public:
	Counter();

	void set_in_load(Pin& in);
	void set_in_data(Pin& in);
	void set_in_count(Pin& in);
	void set_in_clear(Pin& in);
	void set_in_clock(Pin& in);

	Pin& get_out();
};

#endif // _COUNTER_H_