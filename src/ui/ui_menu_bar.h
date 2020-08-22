#ifndef _UI_MENU_BAR_H_
#define _UI_MENU_BAR_H_

#include "ui_element.h"

#include <vector>

class UIMenuBar : public UIElement {
public:
    UIMenuBar();

    virtual void draw() override;
    virtual bool handle_event(S2D_Event e) override;

private:
    std::vector<UIButton*>  m_buttons;

    void add_button(const char* text, const char* icon);
};

#endif // _UI_MENU_BAR_H_