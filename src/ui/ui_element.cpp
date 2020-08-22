#include "ui_element.h"
#include "ui_manager.h"

Vec2 gWindowSize(0, 0);

UIElement::UIElement()
:   m_managed(false)
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

void UIElement::managed(bool managed) {
    if(managed && !m_managed) {
        UIManager::get().add_element(this);
    } else if(!managed && m_managed) {
        UIManager::get().remove_element(this);
    }
}

void UIElement::set_priority(uint16_t priority) {
    m_priority = priority;
    if(m_managed) {
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

void UIElement::set_fill(uint8_t fill) {
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
    m_text_obj = S2D_CreateText("assets/fonts/JetBrainsMono-Regular.ttf", m_text, 14);
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
        case ALIGN_CL:
            m_x_offset = 0;
            m_y_offset = (m_render_rect.height() - m_text_obj->height) / 2;
            break;
        case ALIGN_C:
            m_x_offset = (m_render_rect.width() - m_text_obj->width) / 2;
            m_y_offset = (m_render_rect.height() - m_text_obj->height) / 2;
            break;
        case ALIGN_CR:
            m_x_offset = (m_render_rect.width() - m_text_obj->width);
            m_y_offset = (m_render_rect.height() - m_text_obj->height) / 2;
            break;
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
    m_text_obj = S2D_CreateText("assets/fonts/JetBrainsMono-Regular.ttf", m_text, size);
    set_text_align(m_text_align);
}

UISprite::UISprite(const char* texture) {
    m_texture = S2D_CreateImage(texture);
}

void UISprite::draw() {
    const Rect& rc = m_render_rect;
    m_texture->x = rc.xmin;
    m_texture->y = rc.ymin;
    m_texture->width = rc.width();
    m_texture->height = rc.height();
    S2D_DrawImage(m_texture);
    UIElement::draw();
}

UIButton::UIButton(const char* text, const char* icon)
:   m_label(nullptr)
,   m_icon(nullptr)
{
    if (text != nullptr) {
        m_label = new UILabel(text);
        add_child(m_label);
    }

    if (icon != nullptr) {
        m_icon = new UISprite(icon);
        add_child(m_icon);
    }

    update_render_rect();

    m_button_color = Color(0.1f, 0.1f, 0.1f, 1.0f);
    m_hover_color = Color(0.92f * 0.7f, 0.63f * 0.7f, 0.20f * 0.7f, 1.0f);
    m_pressed_color = Color(0.92f, 0.63f, 0.20f, 1.0f);
}

void UIButton::draw() {
    const Rect& rc = m_render_rect;

    Color c = m_button_color;
    if(m_pressed) {
        c = m_pressed_color;
    } else if(m_mouse_over) {
        c = m_hover_color;
    }

    S2D_DrawQuad(
        rc.xmin, rc.ymin, COLOR2RGBA(c),
        rc.xmin, rc.ymax, COLOR2RGBA(c),
        rc.xmax, rc.ymax, COLOR2RGBA(c),
        rc.xmax, rc.ymin, COLOR2RGBA(c)
    );

    UIElement::draw();
}

void UIButton::update_render_rect() {
    UIElement::update_render_rect();
    
    const Rect& rc = m_render_rect;

    if (m_icon != nullptr && m_label != nullptr) {
        m_label->set_alignment(ALIGN_CL);
        m_label->set_anchor(ALIGN_CL);
        m_label->set_text_align(ALIGN_CL);
        m_label->set_rect(Rect(2, 0, rc.width() - 4, rc.height() - 4));

        m_icon->set_alignment(ALIGN_CR);
        m_icon->set_anchor(ALIGN_CR);
        m_icon->set_rect(Rect(-2, 0, rc.height() - 4, rc.height() - 4));

    } else if (m_icon != nullptr && m_label == nullptr) {
        m_icon->set_alignment(ALIGN_C);
        m_icon->set_anchor(ALIGN_C);

        float size = std::min(rc.height(), rc.width());
        m_icon->set_rect(Rect(0, 0, size - 4, size - 4));

    } else if (m_icon == nullptr && m_label != nullptr) {
        m_label->set_alignment(ALIGN_C);
        m_label->set_anchor(ALIGN_C);
        m_label->set_text_align(ALIGN_C);
        m_label->set_rect(Rect(0, 0, rc.width(), rc.height()));
    }
}

bool UIButton::handle_event(S2D_Event e) {
    if(UIElement::handle_event(e))
        return true;

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
                // Trigger button click callback
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

UIRect::UIRect(float r, float g, float b, float a)
:   r(r), g(g), b(b), a(a) {}

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
