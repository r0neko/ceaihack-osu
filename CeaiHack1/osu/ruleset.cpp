#include "ruleset.h"
#include <stdio.h>

using namespace ceaihack::osu;

ruleset::ruleset(uintptr_t base_address) {
	base = base_address;

	printf("Ruleset @ %08X\n", base);

	if (base == NULL) return;

	// init sprite
	if (*(uintptr_t*)(base + 0x54) != NULL)
		sprite_manager_flashlight = new sprite_manager(*(uintptr_t*)(base + 0x54));
}