#ifndef _UI_TOOL_PANEL_H_
#define _UI_TOOL_PANEL_H_

#include "ui_element.h"

class UIToolPanel : public UIElement {
public:
    UIToolPanel();

    virtual void draw() override;
    virtual bool handle_event(S2D_Event e) override;

private:
    std::vector<UIButton*>  m_buttons;

    void add_button(const char* text, const char* icon, Callback cb = nullptr);
};

#endif // _UI_TOOL_PANEL_H_