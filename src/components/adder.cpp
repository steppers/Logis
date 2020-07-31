#include "adder.h"

#include "../executor.h"

void Adder::on_trigger(Pin* o) {
	out.set_state(in_a->get_state() + in_b->get_state());
}

void Adder::set_inputs(Pin& a, Pin& b) {
	in_a = &a;
	in_b = &b;
	a.register_observer(this);
	b.register_observer(this);
	gExecutor.add_job(this, in_a);
}

Pin& Adder::get_out() {
	return out;
}