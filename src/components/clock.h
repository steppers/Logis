#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "../pin.h"

class Clock : public Pin {
public:
	void	cycle();
};

#endif // _CLOCK_H_