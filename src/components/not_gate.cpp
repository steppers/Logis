#include "not_gate.h"

#include "../executor.h"

void NOTGate::on_trigger(Pin* o) {
	out.set_state(~in_a->get_state());
}

void NOTGate::set_input(Pin& a) {
	in_a = &a;
	a.register_observer(this);
	gExecutor.add_job(this, in_a);
}

Pin& NOTGate::get_out() {
	return out;
}