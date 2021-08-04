#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include "vector2.h"

#include "./osu/object_type.h"
#include "./osu/rulesets.h"
#include "./osu/mods.h"
#include "./osu/modes.h"
#include "./osu/memptr_var.h"

#include "ch_memory_addy.h"

using namespace ceaihack::osu;

// BASS things
#define BASS_ATTRIB_FREQ 1
#define BASS_ATTRIB_TEMPO 0x10000

struct beatmap_t {
    ruleset_e mode;
    float ar;
    float cs;
    float hp;
    float od;
    double slider_multiplier;
    double slider_tick_rate;
};

struct hit_object_t {
    object_type_e type;
    int start;
    int end;
    vector2_t position;
};

namespace ceaihack::cheat::memory {
    // types
    typedef bool(__stdcall* bass_channel_set_attribute_t)(DWORD handle, DWORD attrib, float value);
    typedef uintptr_t(__fastcall* get_audio_stream_t)(void* ecx, void* edx);

    void init();

    inline get_audio_stream_t get_audio_stream;
    //inline void* hax_check_audio;

    inline HMODULE bass_handle;
    inline bass_channel_set_attribute_t bass_channel_set_attribute;

    mode_e get_current_state();
    vector2_t get_cursor_position();
    int get_current_time();

    void set_cursor_position(vector2_t new_pos);
    void set_cursor_position(float x, float y);

    beatmap_t get_loaded_beatmap();
    mod_e get_current_mods();
    bool is_player_loaded();
    bool is_hit_object_manager_loaded();
    float get_current_rate();
    std::vector<hit_object_t> get_beatmap_objects();
    bool get_current_beatmap_object(hit_object_t* current_object);
    vector2_t get_gamefield_origin();
    vector2_t get_relative_pos_to_gamefield(vector2_t position);
    float get_object_radius();

    uintptr_t get_player_base();
    uintptr_t get_ruleset_base();
    uintptr_t get_beatmap_base();
    uintptr_t get_hit_object_manager_base();

    inline ceaihack::utils::memptr_var<int> current_time;
}