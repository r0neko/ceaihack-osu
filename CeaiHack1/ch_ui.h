#pragma once
#include <Windows.h>
#include "imgui.h"

namespace ceaihack::cheat::ui {
	void init(HDC context);
	void on_frame(HDC context);
	void unload();
	void render_tab();

	inline ImFont* font_logo;
	inline ImFont* font_title;
}