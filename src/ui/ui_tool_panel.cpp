#include "ui_tool_panel.h"
#include "../circuit_builder.h"

UIToolPanel::UIToolPanel()
{
    m_priority = 30;
    set_fill(FILL_HEIGHT);
    set_rect(Rect(0, 0, 200, 20));

    add_button("CLOCK", nullptr, [](){ CircuitBuilder::get().set_build_component(CT_CLOCK);});
    add_button("PIN", nullptr, [](){ CircuitBuilder::get().set_build_component(CT_PIN);});
    add_button("AND", nullptr, [](){ CircuitBuilder::get().set_build_component(CT_AND);});
    add_button("OR", nullptr, [](){ CircuitBuilder::get().set_build_component(CT_OR);});
    add_button("ADDER", nullptr, [](){ CircuitBuilder::get().set_build_component(CT_ADDER);});
}

void UIToolPanel::draw() {
    const Rect& rc = m_render_rect;
    S2D_DrawQuad(
        rc.xmin, rc.ymin, 0.15f, 0.15f, 0.15f, 1,
        rc.xmin, rc.ymax, 0.15f, 0.15f, 0.15f, 1,
        rc.xmax, rc.ymax, 0.15f, 0.15f, 0.15f, 1,
        rc.xmax, rc.ymin, 0.15f, 0.15f, 0.15f, 1
    );
    UIElement::draw();
}

bool UIToolPanel::handle_event(S2D_Event e) {
    if(UIElement::handle_event(e))
        return true;

    // Consume mouse down events
    switch(e.type) {
        case S2D_MOUSE_DOWN:
            if (m_render_rect.contains(e.x, e.y)) {
                return true;
            }
            break;
        case S2D_MOUSE_MOVE:
            if(m_render_rect.contains(e.x, e.y)) {
                return true;
            }
            break;
    }

    return false;
}

void UIToolPanel::add_button(const char* text, const char* icon, Callback cb) {
    UIButton* button = new UIButton(text, icon);
    button->set_rect(Rect(0, 20 + (m_buttons.size() * 20), 200, 20));
    button->set_color(Color(0.2f, 0.2f, 0.2f, 1.0f));
    button->bind_on_click(cb);
    add_child(button);

    m_buttons.push_back(button);
}