#include "ch_memory.h"
#include "ch_game_signature.h"
#include "ch_logger.h"

#include "./osu/value_obfuscator.h"

#include <list>
#include <iostream>

void dmp_mem_regions();
uintptr_t search_pattern(const char* pattern, const char* mask);

// memory shit because we like to paste

struct MemoryRegion
{
	DWORD BaseAddress;
	SIZE_T RegionSize;
	DWORD State;
	DWORD Protect;
	DWORD Type;

	MemoryRegion(MEMORY_BASIC_INFORMATION32 mbi)
	{
		BaseAddress = mbi.BaseAddress;
		RegionSize = mbi.RegionSize;
		State = mbi.State;
		Protect = mbi.Protect;
		Type = mbi.Type;
	}
};

std::list<MemoryRegion> memoryRegions;

// end of shit

void ceaihack::cheat::memory::init() {
	dmp_mem_regions();
	ceaihack::memory::location::state_base = *(uintptr_t*) (search_pattern(ceaihack::cheat::signatures::state_signature, "xxxx????x????x????x") + 0x13);
	ceaihack::memory::location::player_base = *(uintptr_t*) (search_pattern(ceaihack::cheat::signatures::player_signature, "xxxxxxx") + 0x7);
	ceaihack::memory::location::set_rate = (void*) (search_pattern(ceaihack::cheat::signatures::set_current_playback_rate_signature, "xxxxxx????xxxxx"));
	ceaihack::memory::location::update_playback_rate = (void*)(search_pattern(ceaihack::cheat::signatures::update_playback_rate_signature, "xxxxxxxxxxxxxxxx"));
	ceaihack::cheat::memory::get_audio_stream = (get_audio_stream_t) search_pattern(ceaihack::cheat::signatures::get_audio_stream_signature, "xxxx");
	ceaihack::memory::location::time_base = *(uintptr_t*)(search_pattern(ceaihack::cheat::signatures::time_signature, "xx????x????xxx????x") + 0x13);
	ceaihack::memory::location::set_cursor_pos = (void*)(search_pattern(ceaihack::cheat::signatures::set_cursor_pos_signature, "xxxxxxx????x"));
	ceaihack::memory::location::gb_time_base = *(uintptr_t*)(search_pattern(ceaihack::cheat::signatures::gamebase_time_signature, "x????xxxxxxxxxxxxxxx") + 0x14);
	ceaihack::memory::location::gb_window_manager_base = *(uintptr_t*)(search_pattern(ceaihack::cheat::signatures::gamebase_windowmanager_signature, "xxx????xxx????xxx????xx") + 0x17);
	//ceaihack::cheat::memory::hax_check_audio = (void*)(search_pattern(ceaihack::cheat::signatures::hax_check_audio_signature, "xxxxxx????xxxxx"));

	ceaihack::cheat::memory::bass_handle = GetModuleHandle(L"bass.dll");

	/*ceaihack::logger::memory->info("BASS handle at {0:x}", ceaihack::cheat::memory::bass_handle);*/

	/*ceaihack::logger::memory->info("state base at {0:x}", ceaihack::cheat::memory::state_base);
	ceaihack::logger::memory->info("player addy at {0:x}", ceaihack::cheat::memory::player_base);
	ceaihack::logger::memory->info("AudioTrack.set_CurrentPlaybackRate at {0:x}", ceaihack::cheat::memory::set_rate);
	ceaihack::logger::memory->info("BASS_ChannelSetAttribute at {0:x}", *ceaihack::cheat::memory::bass_channel_set_attribute);*/

	ceaihack::cheat::memory::current_time = ceaihack::utils::make_memptr_var<int>(ceaihack::memory::location::time_base);
}

bool ceaihack::cheat::memory::is_player_loaded() {
	return get_player_base() != NULL && *(bool*)(get_player_base() + 0x183) == true;
}

bool ceaihack::cheat::memory::is_hit_object_manager_loaded() {
	return is_player_loaded() && get_hit_object_manager_base() != NULL;
}

uintptr_t ceaihack::cheat::memory::get_player_base() {
	return *(uintptr_t*) ceaihack::memory::location::player_base;
}

uintptr_t ceaihack::cheat::memory::get_ruleset_base() {
	return *(uintptr_t*)(get_player_base() + 0x60);
}

uintptr_t ceaihack::cheat::memory::get_beatmap_base() {
	return *(uintptr_t*)(get_player_base() + 0xD4);
}

uintptr_t ceaihack::cheat::memory::get_hit_object_manager_base() {
	return *(uintptr_t*)(get_player_base() + 0x40);
}

int ceaihack::cheat::memory::get_current_time() {
	return *(int*) ceaihack::memory::location::time_base;
}

float get_window_ratio() {
	// PLEASE UPDATE WITH A PROPER WAY OK
	float window_width = 1280;
	float window_height = 720;

	return window_height / 480;
}

float apply_difficulty(float difficulty, float hr = 1.3f) {
	mod_e mods = ceaihack::cheat::memory::get_current_mods();

	if (((int)mods & (int)mod_e::easy) > 0)
		return max(0, difficulty / 2);
	else if (((int)mods & (int)mod_e::hard_rock) > 0)
		return min(10, difficulty * hr);

	return difficulty;
}

float ceaihack::cheat::memory::get_object_radius() {
	if (!is_hit_object_manager_loaded()) return 15;

	float width = 512 * get_window_ratio();
	float difficulty = apply_difficulty(get_loaded_beatmap().cs);

	float sprite_display_size = (width / 8 * (1 - 0.7f * ((difficulty - 5) / 5)));
	return sprite_display_size / 2 / get_window_ratio() * 1.00041f;
}

vector2_t ceaihack::cheat::memory::get_relative_pos_to_gamefield(vector2_t position) {
	return position * get_window_ratio() + get_gamefield_origin();
}

vector2_t ceaihack::cheat::memory::get_gamefield_origin() {
	// PLEASE UPDATE WITH A PROPER WAY OK
	float window_width = 1280;
	float window_height = 720;

	float width = 512 * get_window_ratio();
	float height = 384 * get_window_ratio();

	float num = -16 * get_window_ratio();
	return vector2_t(
		(window_width - width) / 2,
		(window_height - height) / 4 * 3 + num
	);
}

float map_difficulty_range(float difficulty, float min, float mid, float max)
{
	auto d = apply_difficulty(difficulty, 1.4);

	if (d > 5)
		return mid + (max - mid) * (d - 5) / 5;
	if (d < 5)
		return mid - (mid - min) * (5 - d) / 5;
	return mid;
}

float calc_preempt_time() {
	return map_difficulty_range(ceaihack::cheat::memory::get_loaded_beatmap().ar, 1800, 1200, 450);
}

bool ceaihack::cheat::memory::get_current_beatmap_object(hit_object_t* current_object) {
	auto time = get_current_time();
	if (is_hit_object_manager_loaded() && time > 0) {
		auto objects = get_beatmap_objects();
		auto preempt = calc_preempt_time();

		if (objects.size() > 0) {
			for (int i = 0; i < objects.size(); i++) {
				if (time > (objects[i].start - preempt) && time < objects[i].end) {
					*current_object = objects[i];
					return true;
				}
			}
		}
	}

	return false;
}

mod_e ceaihack::cheat::memory::get_current_mods() {
	uintptr_t mods_ptr = *(uintptr_t*)(get_hit_object_manager_base() + 0x34);
	if (mods_ptr == NULL) return (mod_e)0;

	return (mod_e) ceaihack::osu::utils::deobfuscate_int(mods_ptr);
}

float ceaihack::cheat::memory::get_current_rate() {
	mod_e mods = get_current_mods();

	float rate = 1.0f;
	if (((int) mods & (int) mod_e::double_time) > 0) rate = 1.5f;
	else if (((int) mods & (int)mod_e::half_time) > 0) rate = 0.75f;

	return rate;
}

beatmap_t ceaihack::cheat::memory::get_loaded_beatmap() {
	struct beatmap_t beatmap;

	uintptr_t beatmap_base = get_beatmap_base();

	beatmap.mode = *(ruleset_e*) (beatmap_base + 0x114);
	beatmap.ar = *(float*)(beatmap_base + 0x2C);
	beatmap.cs = *(float*)(beatmap_base + 0x30);
	beatmap.hp = *(float*)(beatmap_base + 0x34);
	beatmap.od = *(float*)(beatmap_base + 0x38);

	return beatmap;
}

std::vector<hit_object_t> ceaihack::cheat::memory::get_beatmap_objects() {
	std::vector<hit_object_t> object_list;

	if (is_hit_object_manager_loaded()) {
		uintptr_t hit_object_list_ptr = *(uintptr_t*)(get_hit_object_manager_base() + 0x48);
		if (hit_object_list_ptr != NULL) {
			printf("hit object beatmap addy: %08X\n", hit_object_list_ptr);

			int length = *(int*)(hit_object_list_ptr + 0xc);
			uintptr_t list_ptr = *(uintptr_t*)(hit_object_list_ptr + 0x4);

			if (list_ptr != NULL) {
				for (int i = 0; i < length; i++) {
					hit_object_t current_object;
					uintptr_t hit_object_ptr = *(uintptr_t*)(list_ptr + 0x8 + 0x4 * i);
					if (hit_object_ptr == NULL) continue; 

					current_object.start = *(int*)(hit_object_ptr + 0x10);
					current_object.end = *(int*)(hit_object_ptr + 0x14);
					current_object.type = *(object_type_e*)(hit_object_ptr + 0x18);
					current_object.position.x = *(float*)(hit_object_ptr + 0x38);
					current_object.position.y = *(float*)(hit_object_ptr + 0x3C);

					if ((int)current_object.type & (int)object_type_e::circle > 0) {
						current_object.end = current_object.start + 10;
					}

					object_list.push_back(current_object);
				}
			}
		}
	}

	return object_list;
}

mode_e ceaihack::cheat::memory::get_current_state() {
	return *(mode_e*) ceaihack::memory::location::state_base;
}

vector2_t ceaihack::cheat::memory::get_cursor_position() {
	struct vector2_t pos;
	uintptr_t ruleset_base = get_ruleset_base();

	pos.x = *(float*)(ruleset_base + 0x80);
	pos.y = *(float*)(ruleset_base + 0x84);

	return pos;
}

void ceaihack::cheat::memory::set_cursor_position(vector2_t new_pos) {
	uintptr_t ruleset_base = get_ruleset_base();

	*(float*)(ruleset_base + 0x80) = new_pos.x;
	*(float*)(ruleset_base + 0x84) = new_pos.y;
}

void ceaihack::cheat::memory::set_cursor_position(float x, float y) {
	vector2_t v;

	v.x = x;
	v.y = y;

	set_cursor_position(v);
}

void ceaihack::cheat::memory::dmp_mem_regions() {
	memoryRegions.clear();

	MEMORY_BASIC_INFORMATION32 mbi;
	LPCVOID address = 0;

	while (VirtualQueryEx(GetCurrentProcess(), address, reinterpret_cast<PMEMORY_BASIC_INFORMATION>(&mbi), sizeof(mbi)) != 0)
	{

		if (mbi.State == MEM_COMMIT && mbi.Protect >= 0x10 && mbi.Protect <= 0x80) {
			memoryRegions.push_back(*new MemoryRegion(mbi));
		}
		address = reinterpret_cast<LPCVOID>(mbi.BaseAddress + mbi.RegionSize);
	}
}

uintptr_t ceaihack::cheat::memory::search_pattern(const char* pattern, const char* mask) {
	for (const auto& region : memoryRegions)
	{
		size_t patternLength = strlen(mask);

		for (uintptr_t i = 0; i < region.RegionSize - patternLength; i++)
		{
			bool found = true;
			for (uintptr_t j = 0; j < patternLength; j++)
			{
				if (mask[j] != '?' && pattern[j] != *(char*)(region.BaseAddress + i + j))
				{
					found = false;
					break;
				}
			}

			if (found) {
				ceaihack::logger::cheat->info("Signature at {0:x}!", region.BaseAddress + i);
				return region.BaseAddress + i;
			}
		}
	}

	return NULL;
}