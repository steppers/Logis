#include "ui_element.h"
#include "ui_manager.h"

Vec2 gWindowSize(0, 0);

UIElement::UIElement()
:   m_enabled(false)
,   m_priority(0)
,   m_rect(0, 0, 50, 50)
,   m_render_rect(0, 0, 50, 50)
,   m_align(ALIGN_TL)
,   m_anchor(ALIGN_TL)
,   m_fill(FILL_NONE)
,   m_parent(nullptr)
{}

void UIElement::draw() {
    for (auto c : m_children) {
        c->draw();
    }
}

void UIElement::update() {
    for (auto c : m_children) {
        c->update();
    }
}

bool UIElement::handle_event(S2D_Event e) {
    for (auto elem : m_children) {
        if (elem->handle_event(e))
            return true;
    }
    return false;
}

void UIElement::enable(bool enabled) {
    if(enabled && !m_enabled) {
        UIManager::get().add_element(this);
    } else if(!enabled && m_enabled) {
        UIManager::get().remove_element(this);
    }
}

void UIElement::set_priority(uint16_t priority) {
    m_priority = priority;
    if(m_enabled) {
        UIManager::get().adjust_priorities();
    }
}

void UIElement::set_rect(Rect r) {
    m_rect = r;
    update_render_rect();
}

void UIElement::set_alignment(UIAlignment align) {
    m_align = align;
    update_render_rect();
}

void UIElement::set_anchor(UIAlignment anchor) {
    m_anchor = anchor;
    update_render_rect();
}

void UIElement::set_fill(UIFill fill) {
    m_fill = fill;
    update_render_rect();
}

void UIElement::on_window_resize() {
    update_render_rect();
}

void UIElement::on_parent_change() {
    update_render_rect();
}

void UIElement::update_render_rect() {
    // Calculate new render rect
    Rect pr = (m_parent != nullptr) ? m_parent->get_render_rect() : Rect(0, 0, gWindowSize.x, gWindowSize.y);
    Rect ours = m_rect;

    switch(m_align) {
        case ALIGN_TL: break;
        case ALIGN_TC: ours.move(-ours.width()/2, 0); break;
        case ALIGN_TR: ours.move(-ours.width(), 0); break;
        case ALIGN_CL: ours.move(0, -ours.height()/2); break;
        case ALIGN_C:  ours.move(-ours.width()/2, -ours.height()/2); break;
        case ALIGN_CR: ours.move(-ours.width(), -ours.height()/2); break;
        case ALIGN_BL: ours.move(0, -ours.height()); break;
        case ALIGN_BC: ours.move(-ours.width()/2, -ours.height()); break;
        case ALIGN_BR: ours.move(-ours.width(), -ours.height()); break;
    }

    switch(m_anchor) {
        case ALIGN_TL: ours.move(pr.xmin                , pr.ymin);                 break;
        case ALIGN_TC: ours.move(pr.xmin + pr.width()/2 , pr.ymin);                 break;
        case ALIGN_TR: ours.move(pr.xmax                , pr.ymin);                 break;
        case ALIGN_CL: ours.move(pr.xmin                , pr.ymin + pr.height()/2); break;
        case ALIGN_C:  ours.move(pr.xmin + pr.width()/2 , pr.ymin + pr.height()/2); break;
        case ALIGN_CR: ours.move(pr.xmax                , pr.ymin + pr.height()/2); break;
        case ALIGN_BL: ours.move(pr.xmin                , pr.ymax);                 break;
        case ALIGN_BC: ours.move(pr.xmin + pr.width()/2 , pr.ymax);                 break;
        case ALIGN_BR: ours.move(pr.xmax                , pr.ymax);                 break;
    }

    if (m_fill & FILL_WIDTH) {
        ours.xmin = pr.xmin;
        ours.xmax = pr.xmax;
    }

    if (m_fill & FILL_HEIGHT) {
        ours.ymin = pr.ymin;
        ours.ymax = pr.ymax;
    }

    m_render_rect = ours;

    for (auto c : m_children) {
        c->on_parent_change();
    }
}

void UIElement::add_child(UIElement* child) {
    m_children.push_back(child);
    child->set_parent(this);
}

void UIElement::remove_child(UIElement* child) {
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
        child->set_parent(nullptr);
    }
}

void UIElement::set_parent(UIElement* parent) {
    m_parent = parent;
    update_render_rect();
}

UILabel::UILabel(const char* text)
:   m_text_obj(nullptr)
,   m_text(text)
{
    m_text_obj = S2D_CreateText("assets/JetBrainsMono-Regular.ttf", m_text, 14);
    set_text_align(ALIGN_C);
}

void UILabel::draw() {
    const Rect& rc = m_render_rect;

    m_text_obj->x = rc.xmin + m_x_offset;
    m_text_obj->y = rc.ymin + m_y_offset;

    S2D_DrawText(m_text_obj);
    
    UIElement::draw();
}

void UILabel::update_render_rect() {
    UIElement::update_render_rect();
    set_text_align(m_text_align);
}

void UILabel::set_text_align(UIAlignment align) {
    m_text_align = align;
    switch(m_text_align) {
        case ALIGN_TL:
            m_x_offset = 0;
            m_y_offset = 0;
            break;
        case ALIGN_TC: break;
        case ALIGN_TR: break;
        case ALIGN_CL: break;
        case ALIGN_C:
            m_x_offset = (m_render_rect.width() - m_text_obj->width) / 2;
            m_y_offset = (m_render_rect.height() - m_text_obj->height) / 2;
            break;
        case ALIGN_CR: break;
        case ALIGN_BL: break;
        case ALIGN_BC: break;
        case ALIGN_BR:
            m_x_offset = m_render_rect.width() - m_text_obj->width;
            m_y_offset = m_render_rect.height() - m_text_obj->height;
            break;
    }
}

void UILabel::set_text_size(int size) {
    S2D_FreeText(m_text_obj);
    m_text_obj = S2D_CreateText("assets/JetBrainsMono-Regular.ttf", m_text, size);
    set_text_align(m_text_align);
}

UIRect::UIRect(float r, float g, float b, float a)
:   r(r), g(g), b(b), a(a), is_pressed(false) {}

void UIRect::draw() {
    const Rect& rc = m_render_rect;
    S2D_DrawQuad(
        rc.xmin, rc.ymin, r, g, b, a,
        rc.xmin, rc.ymax, r, g, b, a,
        rc.xmax, rc.ymax, r, g, b, a,
        rc.xmax, rc.ymin, r, g, b, a
    );
    UIElement::draw();
}

bool UIRect::handle_event(S2D_Event e) {
    if(UIElement::handle_event(e))
        return true;

    switch(e.type) {
        case S2D_MOUSE_DOWN:
            if (e.button == S2D_MOUSE_LEFT && m_render_rect.contains(e.x, e.y) && !is_pressed) {
                r *= 1.2f;
                g *= 1.2f;
                b *= 1.2f;
                is_pressed = true;
                return true;
            }
            break;
        case S2D_MOUSE_UP:
            if (e.button == S2D_MOUSE_LEFT && is_pressed) {
                r /= 1.2f;
                g /= 1.2f;
                b /= 1.2f;
                is_pressed = false;
                return true;
            }
            break;
        case S2D_MOUSE_MOVE:
            if (!m_render_rect.contains(e.x, e.y) && is_pressed) {
                r /= 1.2f;
                g /= 1.2f;
                b /= 1.2f;
                is_pressed = false;
                return true;
            }
            break;
    }

    return false;
}