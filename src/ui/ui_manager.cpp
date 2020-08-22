#include "ui_manager.h"

#include <algorithm>

UIManager::UIManager()
:   m_dragged(nullptr)
{}

void UIManager::update() {
    for (auto e : m_elements) {
        e->update();
    }
}

void UIManager::draw() {
    for (auto e : m_elements) {
        e->draw();
    }
}

void UIManager::on_window_resize() {
    for (auto e : m_elements) {
        e->on_window_resize();
    }
}

void UIManager::handle_event(S2D_Event e) {
    // if (m_dragged) {
        
    // }

    // Events handled front to back
    for(auto it = m_elements.rbegin() ;it != m_elements.rend(); ++it) {
        if ((*it)->handle_event(e))
            break;
    }
}

void UIManager::add_element(UIElement* element) {
    m_elements.push_back(element);
    adjust_priorities();
}

void UIManager::remove_element(UIElement* element) {

}

void UIManager::adjust_priorities() {
    std::sort(m_elements.begin(), m_elements.end(), [](UIElement* a, UIElement* b) -> bool {
        return a->get_priority() < b->get_priority();
    });
}