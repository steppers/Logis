#ifndef _UI_WORKSPACE_H_
#define _UI_WORKSPACE_H_

#include "ui_element.h"
#include "../circuit_builder.h"
#include "../circuit.h"

#include <vector>

class UIWorkSpace : public UIElement {
public:
    UIWorkSpace();

    virtual void draw() override;
    virtual bool handle_event(S2D_Event e) override;

private:
    bool        m_pressed;
    bool        m_mouse_over;

    Circuit     m_circuit;
};

#endif // _UI_WORKSPACE_H_