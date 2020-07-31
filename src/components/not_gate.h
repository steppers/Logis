#ifndef _NOT_GATE_H_
#define _NOT_GATE_H_

#include "../pin.h"

class NOTGate : PinObserver {
private:
	Pin* in_a;
	Pin out;

	void on_trigger(Pin* o);

public:
	void set_input(Pin& a);
	Pin& get_out();
};

#endif // _NOT_GATE_H_