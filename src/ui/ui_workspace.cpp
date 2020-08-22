#include "ui_workspace.h"
#include "../pin.h"
#include "../components/and_gate.h"

#include "ui_circuit.h"

#include <stdio.h>

class UIComponent : public UIElement {
public:
    UIComponent()
    :   m_pressed(false)
    ,   m_mouse_over(false)
    ,   m_dragged(false)
    {}

    virtual bool handle_event(S2D_Event e) override {
        switch(e.type) {
            case S2D_MOUSE_DOWN:
                if (e.button == S2D_MOUSE_LEFT && m_mouse_over && !m_pressed) {
                    m_pressed = true;
                    return true;
                }
                break;
            case S2D_MOUSE_UP:
                m_dragged = false;
                if (e.button == S2D_MOUSE_LEFT && m_pressed) {
                    m_pressed = false;
                    return true;
                }
                break;
            case S2D_MOUSE_MOVE:
                m_mouse_over = m_render_rect.contains(e.x, e.y);
                if(m_pressed) {
                    Rect r = m_rect;
                    r.move_center_to(e.x, e.y);
                    set_rect(r);
                    m_dragged = true;
                    return true;
                }
                break;
        }

        return false;
    }

    Pin         m_pin;

protected:
    bool        m_pressed;
    bool        m_mouse_over;
    bool        m_dragged;
};

class UIClock : public UIComponent {
public:
    virtual void draw() override {
        const Rect& rc = m_render_rect;
        Color c = (global_clock.get_state() == 0) ?
            Color(0.2f, 0.6f, 0.2f, 1.0f) :
            Color(0.2f, 0.9f, 0.2f, 1.0f);

        S2D_DrawQuad(
            rc.xmin, rc.ymin, COLOR2RGBA(c),
            rc.xmin, rc.ymax, COLOR2RGBA(c),
            rc.xmax, rc.ymax, COLOR2RGBA(c),
            rc.xmax, rc.ymin, COLOR2RGBA(c)
        );
    }
};

class UIPin : public UIComponent {
public:
    UIPin()
    :   m_last_state(0)
    ,   m_label("0")
    {
        m_pin.set_state(0);

        m_label.set_anchor(ALIGN_C);
        m_label.set_alignment(ALIGN_C);
        add_child(&m_label);
    }

    virtual void update() override {
        uint64_t s = m_pin.get_state();
        if(s != m_last_state) {
            char tmp[17];
            sprintf(tmp, "%x", s);
            tmp[16] = '\0';
            m_label.set_text(tmp);
            m_last_state = s;
        }
    }

    virtual void draw() override {
        const Rect& rc = m_render_rect;
        const Color c = Color(0.3f, 0.3f, 0.3f, 1.0f);
        S2D_DrawQuad(
            rc.xmin, rc.ymin, COLOR2RGBA(c),
            rc.xmin, rc.ymax, COLOR2RGBA(c),
            rc.xmax, rc.ymax, COLOR2RGBA(c),
            rc.xmax, rc.ymin, COLOR2RGBA(c)
        );

        UIElement::draw();
    }

    virtual bool handle_event(S2D_Event e) override {
        switch(e.type) {
            case S2D_MOUSE_UP:
                if (e.button == S2D_MOUSE_LEFT && m_pressed && !m_dragged) {
                    m_pressed = false;
                    // On Click
                    m_pin.set_state(m_last_state == 0 ? 1 : 0);
                    return true;
                }
                break;
        }

        return UIComponent::handle_event(e);
    }

    Pin         m_pin;

private:
    uint64_t    m_last_state;

    UILabel     m_label;
};

class UIAnd : public UIComponent {
public:
    UIAnd()
    :   m_last_state(0)
    ,   m_label("0")
    {
        m_label.set_anchor(ALIGN_C);
        m_label.set_alignment(ALIGN_C);
        add_child(&m_label);
    }

    virtual void update() override {
        uint64_t s = m_gate.get_out().get_state();
        if(s != m_last_state) {
            char tmp[17];
            sprintf(tmp, "%x", s);
            tmp[16] = '\0';
            m_label.set_text(tmp);
            m_last_state = s;
        }
    }

    virtual void draw() override {
        const Rect& rc = m_render_rect;
        const Color c = Color(0.3f, 0.3f, 0.3f, 1.0f);
        S2D_DrawQuad(
            rc.xmin, rc.ymin, COLOR2RGBA(c),
            rc.xmin, rc.ymax, COLOR2RGBA(c),
            rc.xmax, rc.ymax, COLOR2RGBA(c),
            rc.xmax, rc.ymin, COLOR2RGBA(c)
        );

        UIElement::draw();
    }

    ANDGate     m_gate;

private:
    uint64_t    m_last_state;

    UILabel     m_label;
};

UIWorkSpace::UIWorkSpace()
:   m_pressed(false)
,   m_mouse_over(false)
{
    m_priority = 0;
    set_fill(FILL_HEIGHT | FILL_WIDTH);

    UICircuit* circuit = new UICircuit();
    add_child(circuit);
}

void UIWorkSpace::draw() {
    const Rect& rc = m_render_rect;
    S2D_DrawQuad(
        rc.xmin, rc.ymin, 0.25f, 0.25f, 0.25f, 1,
        rc.xmin, rc.ymax, 0.25f, 0.25f, 0.25f, 1,
        rc.xmax, rc.ymax, 0.25f, 0.25f, 0.25f, 1,
        rc.xmax, rc.ymin, 0.25f, 0.25f, 0.25f, 1
    );
    UIElement::draw();
}

bool UIWorkSpace::handle_event(S2D_Event e) {
    if(UIElement::handle_event(e)) {
        CircuitBuilder::get().set_build_component(); // clear
        return true;
    }

    // Consume mouse down events
    switch(e.type) {
        case S2D_MOUSE_DOWN:
            if (e.button == S2D_MOUSE_LEFT && m_mouse_over && !m_pressed) {
                m_pressed = true;
                return true;
            }
            break;
        case S2D_MOUSE_UP:
            if (e.button == S2D_MOUSE_LEFT && m_pressed) {
                m_pressed = false;
                // On Click
                COMPONENT_TYPE comp = CircuitBuilder::get().get_build_type();
                if(comp != CT_NONE) {
                    CircuitBuilder::get().set_build_component(); // clear

                    printf("Add component: %s\n", COMPONENT_TYPE_STRING(comp));

                    UIElement* c = nullptr;
                    switch(comp) {
                        case CT_CLOCK: c = new UIClock(); break;
                        case CT_PIN: c = new UIPin(); break;
                        case CT_AND: c = new UIAnd(); break;
                    }
                    if(c != nullptr) {
                        c->set_rect(Rect(e.x - 15, e.y - 15, 30, 30));
                        add_child(c);
                    }
                }
                return true;
            }
            break;
        case S2D_MOUSE_MOVE:
            m_mouse_over = m_render_rect.contains(e.x, e.y);
            if (!m_mouse_over && m_pressed) {
                m_pressed = false;
                return true;
            }
            break;
    }

    return false;
}
    