#ifndef _ADDER_H_
#define _ADDER_H_

#include "../pin.h"

class Adder : PinObserver {
private:
	Pin* in_a;
	Pin* in_b;
	Pin out;

	void on_trigger(Pin* o);

public:
	void set_inputs(Pin& a, Pin& b);
	Pin& get_out();
};

#endif // _ADDER_H_