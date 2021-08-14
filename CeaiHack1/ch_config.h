#pragma once
#include "ch_game_modifier.h"

enum class ui_tabs {
	timewarp,
	relax,
	aim_assist,
	visuals,
	config,
#ifdef _DEBUG 
	debug
#endif
};

namespace ceaihack::config {
	inline int internal_clock = 0;
	inline bool is_menu_open = false;
	inline bool is_running = true;
	inline ui_tabs current_tab = ui_tabs::timewarp;
#ifdef _DEBUG
	inline bool show_beatmap_objects = false;
#endif

	namespace features::timewarp {
		inline bool enabled = false;
		inline float rate = 1.0f;
		inline double o_rate = 100;
	}

	namespace features::visuals {
		namespace unmod {
			inline bool flashlight = false;
		}
	}

	namespace features::game_modifiers {
		namespace approach_rate {
			inline bool enabled = false;
			inline float new_value = 3.0f;
		}
	}

	namespace features::aimassist {
		inline bool enabled = false;
		inline float radius_start_correction = 1.3f;
		inline float power = 2.7f;
		inline float amplitude = 1.0f;
#ifdef _DEBUG
		inline bool show_prediction = true;
#endif

		// auto aim
		inline bool auto_aim = false;
		inline float auto_interp_time = .35f;
	}
}