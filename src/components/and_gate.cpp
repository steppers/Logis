#include "and_gate.h"

#include "../executor.h"

void ANDGate::on_trigger(Pin* o) {
	out.set_state(in_a->get_state() & in_b->get_state());
}

void ANDGate::set_inputs(Pin& a, Pin& b) {
	in_a = &a;
	in_b = &b;
	a.register_observer(this);
	b.register_observer(this);
	gExecutor.add_job(this, in_a);
}

Pin& ANDGate::get_out() {
	return out;
}