#pragma once

namespace ceaihack::cheat::signatures {
	constexpr auto state_signature = "\x89\x45\xF0\xA1\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xA1\x00\x00\x00\x00\xA3";
	constexpr auto player_signature = "\xFF\x50\x0C\x8B\xD8\x8B\x15";
	constexpr auto set_current_playback_rate_signature = "\x55\x8B\xEC\x56\x8B\x35\x00\x00\x00\x00\x85\xF6\x75\x05\x5E";
	constexpr auto get_audio_stream_signature = "\xDD\x41\x1C\xC3";
	constexpr auto update_playback_rate_signature = "\x55\x8B\xEC\x56\x8B\xF1\x8B\xCE\x8B\x01\x8B\x40\x30\xFF\x50\x14";
	constexpr auto time_signature = "\x2B\x05\x00\x00\x00\x00\xA3\x00\x00\x00\x00\xEB\x0A\xA1\x00\x00\x00\x00\xA3";
	constexpr auto set_cursor_pos_signature = "\x55\x8B\xEC\x83\xEC\x14\xA1\x00\x00\x00\x00\x83";
	constexpr auto gamebase_time_signature = "\xE8\x00\x00\x00\x00\xDD\x5C\x24\x08\xF2\x0F\x10\x44\x24\x08\xF2\x0F\x2C\xC0\xA3";
}