#include "ch_game_modifier.h"
#include "ch_config.h"
#include "ch_memory.h"
#include "ch_game_signature.h"
#include "ch_logger.h"

#include "./osu/osu.h"
#include <stdio.h>

uintptr_t hom_update_variables_addy = NULL;

constexpr auto original_preempt_code = "\xDD\x5D\xE0\xDD\x45\xE0";
constexpr auto new_preempt_code = "\xD9\x05\x00\x00\x00\x00";

void ceaihack::cheat::features::game_modifier::init() {
	hom_update_variables_addy = ceaihack::cheat::memory::search_pattern(ceaihack::cheat::signatures::hom_update_variables_signature, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	printf("GMod: HitObjectManager::UpdateVariables @ %08X\n", hom_update_variables_addy);
}

void ceaihack::cheat::features::game_modifier::update() {
	// if it was not jitted yet just make sure to wait until a proper player instance has been found lol
	if (hom_update_variables_addy == NULL && !osu::player::is_instance()) return;

	// if a player instance exists and the pointer to the jitted function doesn't exist, we go and search for it(aka call init)
	if (hom_update_variables_addy == NULL) {
		ceaihack::cheat::memory::dmp_mem_regions();
		init();
	}

	// if it still doesn't exist, just return out of this.
	if (hom_update_variables_addy == NULL)
		return;

	// now let the fun begin!
	
	// ar changer aka preempt modifier
	uintptr_t preempt_code_location = hom_update_variables_addy + 0x1A7;
	BYTE* preempt_code = (BYTE*) preempt_code_location;

	if (config::features::game_modifiers::approach_rate::enabled) {
		if (*(uint16_t*)(preempt_code_location) != 0x05D9) {
			*(uint16_t*)(preempt_code_location) = 0x05D9;
			*(uintptr_t*)(preempt_code_location + 0x2) = (uintptr_t) &config::features::game_modifiers::approach_rate::new_value;
			ceaihack::logger::features->info("Patched the AR getter!");
		}
	} else {
		if (memcmp(preempt_code, (BYTE*)original_preempt_code, 6) != 0) {
			memcpy(preempt_code, (BYTE*)original_preempt_code, 6);
			ceaihack::logger::features->info("Restored Original AR Getter!");
		}
	}
}