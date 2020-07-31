#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "../pin.h"

class Buffer : PinObserver {
private:
	Pin* in_a; // data
	Pin* in_b; // enable
	Pin out;

	void on_trigger(Pin* o);

public:
	void set_inputs(Pin& a, Pin& b);
	Pin& get_out();
};

#endif // _BUFFER_H_