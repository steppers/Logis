#include "circuit.h"

Clock global_clock;

Circuit::Circuit() {
    
}

void Circuit::tick() {
    global_clock.cycle();
    gExecutor.run();
}

void Circuit::add_component(PinObserver* comp) {
    m_index_map[comp] = m_components.size();
    m_components.push_back(comp);
}

void Circuit::remove_component(PinObserver* comp) {
    size_t index = m_index_map[comp];

    auto end = m_components.end();

    // Remove from index index
    m_index_map[*end] = index;
    m_index_map.erase(comp);

    // Remove from components
    m_components[index] = *end;
    m_components.pop_back();
}

PinObserver* Circuit::get_component(size_t index) {
    return m_components[index];
}