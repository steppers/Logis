#include "buffer.h"

#include "../executor.h"

void Buffer::on_trigger(Pin* o) {
	// TODO
}

void Buffer::set_inputs(Pin& a, Pin& b) {
	in_a = &a;
	in_b = &b;
	a.register_observer(this);
	b.register_observer(this);
	gExecutor.add_job(this, in_a);
}

Pin& Buffer::get_out() {
	return out;
}