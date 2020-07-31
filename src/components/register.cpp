#include "register.h"

#include "../executor.h"

void Register::on_trigger(Pin* o) {
	if (o == in_clock && o->get_state() == 1 && in_write->get_state() == 1) {
		out_data.set_state(in_data->get_state());
	}
}

void Register::set_in_data(Pin& in) {
	in_data = &in;
	in.register_observer(this);
}

void Register::set_in_write(Pin& in) {
	in_write = &in;
	in.register_observer(this);
}

void Register::set_in_clock(Pin& in) {
	in_clock = &in;
	in.register_observer(this);
}

Pin& Register::get_out() {
	return out_data;
}