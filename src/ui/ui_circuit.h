#ifndef _UI_CIRCUIT_H_
#define _UI_CIRCUIT_H_

#include "ui_element.h"
#include "../circuit.h"

#include <vector>

struct PinDef {
    Pin*        pin;
    uint32_t    x, y;
};

class UICircuitComponent : public UIElement {
public:
    UICircuitComponent(uint32_t width, uint32_t height);

    virtual void draw() override;
    virtual bool handle_event(S2D_Event e) override;

    void add_pin(uint32_t x, uint32_t y, Pin* pin = nullptr);

protected:
    std::vector<PinDef> m_pins;

    bool        m_pressed;
    bool        m_mouse_over;
    bool        m_dragged;
};

class UICircuit : public UIElement {
public:
    UICircuit();

    virtual void draw() override;
    virtual bool handle_event(S2D_Event e) override;

private:
    bool        m_pressed;
    bool        m_mouse_over;

    Circuit     m_circuit;
};

#endif // _UI_CIRCUIT_H_