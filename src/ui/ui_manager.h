#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "ui_element.h"

class UIManager {
public:
    static UIManager& get() {
        static UIManager instance;
        return instance;
    }

    void update();
    void draw();

    void add_element(UIElement* element);
    void remove_element(UIElement* element);

    void adjust_priorities();

    void handle_event(S2D_Event e);
    void on_window_resize();

private:
    std::vector<UIElement*> m_elements;

private:
    UIManager() {};
};

#endif // _UI_MANAGER_H_