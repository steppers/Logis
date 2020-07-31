#include "counter.h"

#include "../executor.h"

Counter::Counter()
:	in_load(nullptr)
,	in_data(nullptr)
,	in_count(nullptr)
,	in_clock(nullptr)
,	in_clear(nullptr)
{}

void Counter::on_trigger(Pin* o) {
	if (o == in_clock && o->get_state() == 1) {

		if (in_load == nullptr || in_load->get_state() == 0) {
			if (in_count == nullptr || in_count->get_state() == 1) {
				out.set_state(out.get_state() + 1);
			}
		} else {

		}

	}
}

void Counter::set_in_load(Pin& in) {
	in_clear = &in;
	in.register_observer(this);
}

void Counter::set_in_data(Pin& in) {
	in_data = &in;
	in.register_observer(this);
}

void Counter::set_in_count(Pin& in) {
	in_clear = &in;
	in.register_observer(this);
}

void Counter::set_in_clear(Pin& in) {
	in_clear = &in;
	in.register_observer(this);
}

void Counter::set_in_clock(Pin& in) {
	in_clock = &in;
	in.register_observer(this);
}

Pin& Counter::get_out() {
	return out;
}