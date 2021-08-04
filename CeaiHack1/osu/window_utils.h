#pragma once
#include "../vector2.h"

struct screen_t {
	float width, height;
};

namespace ceaihack::utils {
	inline screen_t get_screen() {
		// PLEASE UPDATE WITH A PROPER WAY OK
		return screen_t { 1280, 720 };
	}

	inline float get_window_ratio() {
		return get_screen().height / 480;
	}

	inline vector2_t get_gamefield_origin() {
		float width = 512 * get_window_ratio();
		float height = 384 * get_window_ratio();

		auto window = get_screen();

		float num = -16 * get_window_ratio();
		return vector2_t(
			(window.width - width) / 2,
			(window.height - height) / 4 * 3 + num
		);
	}

	inline vector2_t get_relative_pos_to_gamefield(vector2_t position) {
		return position * get_window_ratio() + get_gamefield_origin();
	}
}