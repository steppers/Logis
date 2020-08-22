#include "ui_menu_bar.h"

UIMenuBar::UIMenuBar()
{
    m_priority = 100;
    set_fill(FILL_WIDTH);
    set_rect(Rect(0, 0, 100, 20));

    add_button("Logis", nullptr);
    add_button("File", nullptr);
    add_button("Edit", nullptr);
}

void UIMenuBar::draw() {
    const Rect& rc = m_render_rect;
    S2D_DrawQuad(
        rc.xmin, rc.ymin, 0.1f, 0.1f, 0.1f, 1,
        rc.xmin, rc.ymax, 0.1f, 0.1f, 0.1f, 1,
        rc.xmax, rc.ymax, 0.1f, 0.1f, 0.1f, 1,
        rc.xmax, rc.ymin, 0.1f, 0.1f, 0.1f, 1
    );
    UIElement::draw();
}

bool UIMenuBar::handle_event(S2D_Event e) {
    if(UIElement::handle_event(e))
        return true;

    // Consume mouse down events
    switch(e.type) {
        case S2D_MOUSE_DOWN:
            if (m_render_rect.contains(e.x, e.y)) {
                return true;
            }
            break;
    }

    return false;
}

void UIMenuBar::add_button(const char* text, const char* icon) {
    UIButton* button = new UIButton(text, icon);
    button->set_alignment(ALIGN_CL);
    button->set_anchor(ALIGN_CL);
    button->set_rect(Rect((m_buttons.size() * 50), 0, 50, 20));
    add_child(button);

    m_buttons.push_back(button);
}