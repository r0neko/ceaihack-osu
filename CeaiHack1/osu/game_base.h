#pragma once
#include "memptr_var.h"


namespace ceaihack::osu {
	class gamebase {
	public:
		gamebase() {}
		~gamebase() {}

		int get_time();

		int get_game_time();

		static gamebase* get_instance();	
	};
}