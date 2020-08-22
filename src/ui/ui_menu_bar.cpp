#include "ui_menu_bar.h"

UIMenuBar::UIMenuBar()
{
    m_priority = 100;
    set_fill(FILL_WIDTH);
    set_rect(Rect(0, 0, 100, 20));
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