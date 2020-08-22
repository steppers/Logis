#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include "components/clock.h"
#include "pin.h"

#include <vector>
#include <unordered_map>

class Circuit {
public:
    Circuit();

    void tick();

    void add_component(PinObserver* comp);
    void remove_component(PinObserver* comp);
    PinObserver* get_component(size_t index);

private:
    std::vector<PinObserver*>                   m_components;
    std::unordered_map<PinObserver*, size_t>    m_index_map;
};

extern Clock global_clock;

#endif // _CIRCUIT_H_