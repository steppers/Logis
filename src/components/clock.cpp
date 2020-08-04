#include "clock.h"

void Clock::cycle() {
	set_state( !m_state );
}