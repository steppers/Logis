#ifndef _UI_ELEMENT_H_
#define _UI_ELEMENT_H_

#include <vector>
#include <simple2d.h>

enum UIAlignment {
    ALIGN_TL,
    ALIGN_TC,
    ALIGN_TR,
    ALIGN_CL,
    ALIGN_C,
    ALIGN_CR,
    ALIGN_BL,
    ALIGN_BC,
    ALIGN_BR
};

enum UIFill {
    FILL_NONE = 0,
    FILL_WIDTH = 1 << 0,
    FILL_HEIGHT = 1 << 1,
};

struct Color {
    float r, g, b, a;
    Color() : r(1), g(1), b(1), a(1) {}
    Color(float ri, float gi, float bi, float ai) 
    :   r(ri), g(gi), b(bi), a(ai) {}
};
#define COLOR2RGB(color) color.r, color.g, color.b
#define COLOR2RGBA(color) color.r, color.g, color.b, color.a
#define COLOR2RGBMUL(color, mul) color.r * mul, color.g * mul, color.b * mul
#define COLOR2RGBAMUL(color, mul) color.r * mul, color.g * mul, color.b * mul, color.a * mul

struct Vec2 {
    float x, y;
    Vec2(float x, float y) : x(x), y(y) {}
};

struct Rect {
    float xmin, ymin, xmax, ymax;

    Rect(float x, float y, float width, float height) 
    :   xmin(x), ymin(y), xmax(x + width), ymax(y + height) {}

    float width() const {
        return xmax - xmin;
    }
    float height() const {
        return ymax - ymin;
    }

    void move(float x, float y) {
        xmin += x; xmax += x;
        ymin += y; ymax += y;
    }

    bool contains(float x, float y) const {
        return x >= xmin && x <= xmax && y >= ymin && y <= ymax;
    }
};

class UIElement {
public:
    UIElement();

    virtual void draw();
    virtual void update();
    virtual bool handle_event(S2D_Event e);

    void            managed(bool managed);

    void            set_priority(uint16_t priority);
    uint16_t        get_priority() { return m_priority; }

    Rect            get_rect() { return m_rect; }
    void            set_rect(Rect r);

    UIAlignment     get_alignment() { return m_align; }
    void            set_alignment(UIAlignment align);

    UIAlignment     get_anchor() { return m_anchor; }
    void            set_anchor(UIAlignment anchor);

    uint8_t         get_fill() { return m_fill; }
    void            set_fill(uint8_t fill);

    Rect            get_render_rect() { return m_render_rect; }

    void            add_child(UIElement* child);
    void            remove_child(UIElement* child);

    void            set_parent(UIElement* parent);

    void            on_window_resize();

protected:
    bool            m_managed;
    int16_t         m_priority;

    Rect            m_rect;
    Rect            m_render_rect;

    UIAlignment     m_align;
    UIAlignment     m_anchor;

    uint8_t         m_fill;

    UIElement*                  m_parent;
    std::vector<UIElement*>     m_children;

    void            on_parent_change();

    virtual void    update_render_rect();
};

class UILabel : public UIElement {
public:
    UILabel(const char* text);

    virtual void draw() override;
    void set_text_align(UIAlignment align);
    void set_text_size(int size);

protected:
    void update_render_rect() override;

private:
    S2D_Text* m_text_obj;
    const char* m_text;

    float m_x_offset, m_y_offset;
    UIAlignment m_text_align;
};

class UISprite : public UIElement {
public:
    UISprite(const char* texture);

    virtual void draw() override;

private:
    S2D_Image*      m_texture;
};

class UIButton : public UIElement {
public:
    UIButton();
    UIButton(const char* text, const char* icon);

    virtual void draw() override;
    virtual bool handle_event(S2D_Event e) override;

    void set_color(Color c);

protected:
    void update_render_rect() override;

private:
    Color       m_button_color;
    Color       m_hover_color;
    Color       m_pressed_color;

    bool        m_pressed;
    bool        m_mouse_over;

    UILabel*    m_label;
    UISprite*   m_icon;
};

class UIRect : public UIElement {
public:
    UIRect(float r, float g, float b, float a);

    virtual void draw() override;

    float r, g, b, a;
};

extern Vec2 gWindowSize;

#endif // _UI_ELEMENT_H_