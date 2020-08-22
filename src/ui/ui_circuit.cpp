#include "ui_circuit.h"
#include "../circuit_builder.h"

UICircuitComponent::UICircuitComponent(uint32_t width, uint32_t height)
{
    set_rect(Rect(0, 0, width, height));
    set_alignment(ALIGN_C);
}

void UICircuitComponent::draw() {
    const Rect& rc = m_render_rect;
    Color c = Color(0.5f, 0.5f, 0.5f, 1.0f);
    S2D_DrawQuad(
        rc.xmin, rc.ymin, COLOR2RGBA(c),
        rc.xmin, rc.ymax, COLOR2RGBA(c),
        rc.xmax, rc.ymax, COLOR2RGBA(c),
        rc.xmax, rc.ymin, COLOR2RGBA(c)
    );

    for(auto p : m_pins) {
        S2D_DrawCircle(p.x + rc.xmin, p.y + rc.ymin, 3, 8, 0, 0.2f, 0, 1);
    }
}

bool UICircuitComponent::handle_event(S2D_Event e) {
    switch(e.type) {
        case S2D_MOUSE_DOWN:
            if (e.button == S2D_MOUSE_LEFT) {
                //Scan pins
                Rect r(0, 0, 6, 6);
                for(auto p : m_pins) {
                    r.move_center_to(p.x + m_render_rect.xmin, p.y + m_render_rect.ymin);
                    if(r.contains(e.x, e.y)) {
                        // Start pin drag
                        return true;
                    }
                }

                // Drag handling
                if (m_mouse_over && !m_pressed) {
                    m_pressed = true;
                    return true;
                }
            }
            break;
        case S2D_MOUSE_UP:
            // Drag handling
            m_dragged = false;
            if (e.button == S2D_MOUSE_LEFT && m_pressed) {
                m_pressed = false;
                return true;
            }
            break;
        case S2D_MOUSE_MOVE:
            m_mouse_over = m_render_rect.contains(e.x, e.y);
            if(m_pressed) {
                // Move in steps of 10 at a time
                uint32_t x = ((e.x / 10) * 10) + 5;
                uint32_t y = ((e.y / 10) * 10) + 5;
                
                move_to(x, y);
                m_dragged = true;
                return true;
            }
            break;
    }

    return false;
}

void UICircuitComponent::add_pin(uint32_t x, uint32_t y, Pin* pin) {
    m_pins.push_back({pin, x, y});
}

UICircuit::UICircuit() {
    set_fill(FILL_WIDTH | FILL_HEIGHT);

    UICircuitComponent* cc = new UICircuitComponent(40, 40);
    cc->move_to(450, 300);
    add_child(cc);

    cc->add_pin(10, 0);
    cc->add_pin(30, 0);
    cc->add_pin(20, 40);
}

void UICircuit::draw() {
    const Rect& rc = m_render_rect;
    S2D_DrawQuad(
        rc.xmin, rc.ymin, 0.25f, 0.25f, 0.25f, 1,
        rc.xmin, rc.ymax, 0.25f, 0.25f, 0.25f, 1,
        rc.xmax, rc.ymax, 0.25f, 0.25f, 0.25f, 1,
        rc.xmax, rc.ymin, 0.25f, 0.25f, 0.25f, 1
    );
    UIElement::draw();
}

bool UICircuit::handle_event(S2D_Event e) {
    if(UIElement::handle_event(e)) {
        CircuitBuilder::get().set_build_component(); // clear
        return true;
    }
    return false;
}
