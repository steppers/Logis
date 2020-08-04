#include <iostream>

#include "pin.h"
#include "executor.h"
#include "components/clock.h"
#include "components/adder.h"
#include "components/register.h"
#include "components/counter.h"

#define PRINT_PIN(pin) \
	std::cout << "[PIN] " # pin ": " << pin.get_state() << std::endl;

int main()
{
	Clock clk;

	Counter cnt1;
	Counter cnt2;
	cnt1.set_in_clock(clk);
	cnt2.set_in_clock(clk);

	Adder add;
	add.set_inputs(cnt1.get_out(), cnt2.get_out());

	PRINT_PIN(add.get_out());

	for(uint32_t i = 0; i < 10000000; ++i) {
		clk.cycle();
		gExecutor.run();
		clk.cycle();
		gExecutor.run();
	}

	PRINT_PIN(add.get_out()); 
}
