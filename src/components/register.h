#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "../pin.h"

class Register : PinObserver {
private:
	Pin* in_data;
	Pin* in_write;
	Pin* in_clock;
	Pin  out_data;

	void on_trigger(Pin* o);

public:
	void set_in_data(Pin& in);
	void set_in_write(Pin& in);
	void set_in_clock(Pin& in);
	Pin& get_out();
};

#endif // _REGISTER_H_