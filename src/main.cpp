#include "ui/ui_element.h"
#include "ui/ui_manager.h"
#include "ui/ui_menu_bar.h"

static S2D_Window* sWindow;

void ui_update() {
    if (gWindowSize.x != sWindow->width || gWindowSize.y != sWindow->height) {
        gWindowSize.x = sWindow->width;
        gWindowSize.y = sWindow->height;
        UIManager::get().on_window_resize();
    }

    UIManager::get().update();
}

void ui_render() {
    UIManager::get().draw();
}

void on_event(S2D_Event e) {
    UIManager::get().handle_event(e);
}

void init_ui() {
	UIMenuBar* menu_bar = new UIMenuBar(); 
	menu_bar->managed(true);

	UIRect* panel = new UIRect(0.2f, 0.2f, 0.2f, 1.0f);
    panel->set_rect(Rect(0, 0, 200, 50));
    panel->set_fill(FILL_HEIGHT);
    panel->managed(true);
}

// #define PRINT_PIN(pin) \
// 	std::cout << "[PIN] " # pin ": " << pin.get_state() << std::endl;

int main()
{
	// Clock clk;

	// Counter cnt1;
	// Counter cnt2;
	// cnt1.set_in_clock(clk);
	// cnt2.set_in_clock(clk);

	// Adder add;
	// add.set_inputs(cnt1.get_out(), cnt2.get_out());

	// PRINT_PIN(add.get_out());

	// for(uint32_t i = 0; i < 1000000; ++i) {
	// 	clk.cycle();
	// 	gExecutor.run();
	// 	clk.cycle();
	// 	gExecutor.run();
	// }

	// PRINT_PIN(add.get_out());
    
	// Create our application window
    sWindow = S2D_CreateWindow(
		"Hello Triangle",
		1280, 800,
		ui_update, ui_render,
		S2D_HIGHDPI | S2D_RESIZABLE
	);
	sWindow->fps_cap = 30;
    sWindow->viewport.mode = S2D_EXPAND;
	sWindow->on_mouse = on_event;
	sWindow->on_key = on_event;

	// Set initial window size
    gWindowSize.x = sWindow->width;
    gWindowSize.y = sWindow->height;

	// Initialise UI
	init_ui();

	// Show the application window
	S2D_Show(sWindow);
	S2D_FreeWindow(sWindow);
}
