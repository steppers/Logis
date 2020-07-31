#ifndef _XOR_GATE_H_
#define _XOR_GATE_H_

#include "../pin.h"

class XORGate : PinObserver {
private:
	Pin* in_a;
	Pin* in_b;
	Pin out;

	void on_trigger(Pin* o);

public:
	void set_inputs(Pin& a, Pin& b);
	Pin& get_out();
};

#endif // _XOR_GATE_H_