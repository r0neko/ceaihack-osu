#pragma once

#include "Windows.h"
#include "sprite_manager.h"

namespace ceaihack::osu {
	class ruleset {
	public:
		ruleset() {}
		ruleset(uintptr_t base_address);
		~ruleset() {}

		sprite_manager* sprite_manager_flashlight;
	private:
		uintptr_t base = NULL;
	};
}