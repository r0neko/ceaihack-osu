#include "ch_timewarp.h"

#include "ch_hooks_vars.h"
#include "ch_config.h"
#include "ch_logger.h"

#include <iostream>

void __stdcall ceaihack::cheat::features::timewarp::rate_change_hook(double rate) {
	ceaihack::logger::features->debug("set_CurrentPlaybackRate called! Rate = {}", rate);
	ceaihack::config::features::timewarp::o_rate = rate;

	if (ceaihack::config::features::timewarp::enabled) {
		ceaihack::logger::features->debug("Timewarp enabled. Forcing new rate to be {}", (double)(ceaihack::config::features::timewarp::rate * 100));
		ceaihack::hooks::variables::o_set_current_playback_rate(ceaihack::config::features::timewarp::rate * 100);
		return;
	}

	ceaihack::hooks::variables::o_set_current_playback_rate(rate);
}

void __stdcall ceaihack::cheat::features::timewarp::hax_check_audio_hook() {
	ceaihack::logger::features->debug("Player::HaxCheckAudio called!");

	if (!ceaihack::config::features::timewarp::enabled)
		ceaihack::hooks::variables::o_hax_check_audio();
}