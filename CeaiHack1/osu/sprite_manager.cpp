#include "sprite_manager.h"
#include <stdio.h>

using namespace ceaihack::osu;

sprite_manager::sprite_manager(uintptr_t base_address) {
	base = base_address;

	printf("SpriteManager @ %08X\n", base);

	if (base == NULL) return;

	alpha = make_memptr_var<float>(base + 0x28);
}