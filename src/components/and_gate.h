#ifndef _AND_GATE_H_
#define _AND_GATE_H_

#include "../pin.h"

class ANDGate : PinObserver {
private:
	Pin* in_a;
	Pin* in_b;
	Pin out;

	void on_trigger(Pin* o);

public:
	void set_inputs(Pin& a, Pin& b);
	Pin& get_out();
};

#endif // _AND_GATE_H_