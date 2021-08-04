#include "ch_ui_design.h"
#include "ch_config.h"
#include "ch_memory.h"
#include "ch_ui.h"
#include "ch_auth.h"
#include "ch_clock.h"

#include "imgui.h"

#include <iostream>

#include "./osu/osu.h"

ImColor ceaihack::cheat::ui::design::user_role_to_color() {
	// lazy to do 1000 calls just do one
	ch_user_role_e role = ceaihack::cheat::auth::auth_data.role;

	if ((role & Developer) >= 0) return ceaihack::cheat::ui::color::role_developer;
	if ((role & VIP) >= 0) return ceaihack::cheat::ui::color::role_vip;

	return ceaihack::cheat::ui::color::role_member;
}

std::string ceaihack::cheat::ui::design::user_role_to_string() {
	// lazy to do 1000 calls just do one
	ch_user_role_e role = ceaihack::cheat::auth::auth_data.role;

	if ((role & Developer) >= 0) return "Developer";
	if ((role & VIP) >= 0) return "VIP";

	return "Member";
}

void tab_button(std::string label, ui_tabs tab) {
	bool do_pop = false;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	if (ceaihack::config::current_tab == tab) {
		ImGui::PushStyleColor(ImGuiCol_Button, ceaihack::cheat::ui::color::tab_selected.Value);
		do_pop = true;
	}

	if (ImGui::Button(label.c_str(), ceaihack::cheat::ui::color::tab_btn_size))
		ceaihack::config::current_tab = tab;

	if (do_pop)
		ImGui::PopStyleColor(1);
	ImGui::PopStyleVar(1);
}

void ceaihack::cheat::ui::design::tabs() {
	ImGui::BeginGroup();
		tab_button("Timewarp", ui_tabs::timewarp);
		tab_button("Visuals", ui_tabs::visuals);
		tab_button("Aim Assist", ui_tabs::aim_assist);
		tab_button("Config", ui_tabs::config);
#if _DEBUG
		tab_button("Debug", ui_tabs::debug);
#endif
	ImGui::EndGroup();
}

void ceaihack::cheat::ui::design::header() {
	ImGui::BeginGroup();
		ImGui::PushFont(ceaihack::cheat::ui::font_logo);
		ImGui::Text("CeaiHack");
		ImGui::PopFont();

		ImGui::SameLine(0, -0.4);

		ImGui::Text("for osu!");

#ifdef _DEBUG
		ImGui::SameLine(0, -0.3f);

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ImGui::Text("(DEBUG VERSION)");
		ImGui::PopStyleColor(1);
#endif
	ImGui::EndGroup();
}

void ceaihack::cheat::ui::design::timewarp() {
	ImGui::PushFont(ceaihack::cheat::ui::font_title);
	ImGui::Text("Modifier Settings");
	ImGui::PopFont();

	ImGui::Checkbox("Enable Timewarp", &ceaihack::config::features::timewarp::enabled);

	if (ceaihack::config::features::timewarp::enabled)
		ImGui::SliderFloat("Timewarp rate", &ceaihack::config::features::timewarp::rate, 0.2f, 1.5f);
}

void ceaihack::cheat::ui::design::visuals() {
	ImGui::PushFont(ceaihack::cheat::ui::font_title);
	ImGui::Text("Unmod");
	ImGui::PopFont();

	ImGui::Checkbox("Remove Flashlight", &ceaihack::config::features::visuals::unmod::flashlight);
}

void ceaihack::cheat::ui::design::aim_assist() {
	ImGui::PushFont(ceaihack::cheat::ui::font_title);
	ImGui::Text("Modifier Settings");
	ImGui::PopFont();

	ImGui::Checkbox("Enable Aim Assist", &ceaihack::config::features::aimassist::enabled);
	
	if (ceaihack::config::features::aimassist::enabled) {
		ImGui::Checkbox("Auto-Aim (NOT RECOMMENDED!)", &ceaihack::config::features::aimassist::auto_aim);
		if (!ceaihack::config::features::aimassist::auto_aim) {
			ImGui::SliderFloat("Assist Radius", &ceaihack::config::features::aimassist::radius_start_correction, 1.5f, 3.0f);
			ImGui::SliderFloat("Assist Power", &ceaihack::config::features::aimassist::power, 1, 100);
#ifdef _DEBUG
			ImGui::Checkbox("Show Predicted Position (DEBUG ONLY)", &ceaihack::config::features::aimassist::show_prediction);
#endif
		}
		else {
			ImGui::SliderFloat("Interpolation Time", &ceaihack::config::features::aimassist::auto_interp_time, 0.2f, 15.0f);
		}
	}
}

#ifdef _DEBUG 
bool b_show_offsets = false;

void ceaihack::cheat::ui::design::debug() {
	ImGui::Text("Debug");
	ImGui::Checkbox("Show Debug Offsets", &b_show_offsets);
	ImGui::Checkbox("Show Beatmap Objects Window", &ceaihack::config::show_beatmap_objects);

	ImGui::Text("Game is running at %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Internal Clock: %lld", ceaihack::cheat::clock::get_current_time());

	ImGui::Text("AudioEngine.Time = %i", ceaihack::cheat::memory::current_time.get());
	ImGui::Text("GameBase.Time = %i", osu::gamebase::get_instance()->get_game_time());

	ImGui::Text("Current game state - %i", ceaihack::cheat::memory::get_current_state());
	ImGui::Text("IsPlayerLoaded - %i", ceaihack::cheat::memory::is_player_loaded());
	
	if (ceaihack::osu::player::is_instance()) {
		vector2_t pos = ceaihack::cheat::memory::get_cursor_position();
		ImGui::Text("Cursor position: %f, %f", pos.x, pos.y);

		beatmap_t beatmap = ceaihack::cheat::memory::get_loaded_beatmap();
		ImGui::Text("Current beatmap: mode %i, AR %.2f, CS %.2f, OD %.2f, HP %.2F", beatmap.mode, beatmap.ar, beatmap.cs, beatmap.od, beatmap.hp);
	}

	if (b_show_offsets) {
		ImGui::Separator();

		ImGui::Text("Offsets");
		ImGui::Text("State Base - 0x%X", ceaihack::memory::location::state_base);
		ImGui::Text("Time Base - 0x%X", ceaihack::memory::location::time_base);
		ImGui::Text("Player Addy - 0x%X", ceaihack::memory::location::player_base);
		ImGui::Text("(static) GameBase.Time - 0x%X", ceaihack::memory::location::gb_time_base);
		if (ceaihack::osu::player::is_instance()) {
			ImGui::Text("Player Base - 0x%X", ceaihack::cheat::memory::get_player_base());
			ImGui::Text("Ruleset Base - 0x%X", ceaihack::cheat::memory::get_ruleset_base());
			ImGui::Text("Beatmap Base - 0x%X", ceaihack::cheat::memory::get_beatmap_base());
			ImGui::Text("HitObjectManager Base - 0x%X", ceaihack::cheat::memory::get_hit_object_manager_base());
		}
	}

	if (ImGui::Button("Unload CeaiHack")) {
		ceaihack::config::current_tab = ui_tabs::timewarp;
		ceaihack::config::is_running = false;
	}

	if (ImGui::Button("Fuck CeaiHack Auth (tm)")) {
		ceaihack::cheat::auth::auth_data.check = 0x727;
	}
}

void ceaihack::cheat::ui::design::beatmap_debug() {
	if (ceaihack::osu::player::is_instance()) {
		auto player = ceaihack::osu::player::instance();

		if (player->hom == nullptr) {
			ImGui::Text("player->hom == null");
			return;
		}

		if (player->hom->beatmap == nullptr) {
			ImGui::Text("player->hom->beatmap == null");
			return;
		}

		if (!player->hom->hit_objects.is_valid()) {
			ImGui::Text("player->hom->hit_objects == null");
			return;
		}

		ImGui::Text("Mods: %i | Rate: %0.2f", player->hom->active_mods.get(), ceaihack::cheat::memory::get_current_rate());
		ImGui::Text("Time: %i", ceaihack::cheat::memory::current_time.get());

		ImGui::Text("AR: %.2f, CS: %.2f, HP: %.2f, OD: %.2f", player->hom->beatmap->approach_rate.get(), player->hom->beatmap->circle_size.get(), player->hom->beatmap->drain_rate.get(), player->hom->beatmap->overall_diff.get());

		ImGui::Text("new HOM Object Count: %i", player->hom->hit_objects.length);

		ImGui::Spacing();

		auto current_object = player->hom->get_current_object();

		if (current_object != nullptr) {
			ImGui::Text("CURRENT type %i, pos(%0.1f, %0.1f), start %i, end %i", current_object->type.get(), current_object->base_position.get().x, current_object->base_position.get().y, current_object->start_time.get(), current_object->end_time.get());
		}
		/*

		ImGui::Spacing();

		hit_object_t current_object;

		if (ceaihack::cheat::memory::get_current_beatmap_object(&current_object)) {
			ImGui::Text("CURRENT type %i, pos(%0.1f, %0.1f), start %i, end %i", current_object.type, current_object.position.x, current_object.position.y, current_object.start, current_object.end);
		}*/

		/*auto time = ceaihack::cheat::memory::get_current_time();

		if (list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				if (time > list[i].start || time < list[i].start) continue;

				if (time > list[i].start && time < list[i].end) {
					ImGui::Text("Current: #%i; type %i, pos(%0.1f, %0.1f), start %i, end %i", i, list[i].type, list[i].position.x, list[i].position.y, list[i].start, list[i].end);
					break;
				}
			}
			
			for (int i = 0; i < list.size(); i++) {
				ImGui::Text("#%i; type %i, pos(%0.1f, %0.1f), start %i, end %i %s", i, list[i].type, list[i].position.x, list[i].position.y, list[i].start, list[i].end, (time > list[i].start && time < list[i].end) ? " <<<" : "");
			}
		}*/
	}
	else {
		ImGui::Text("No player instance found in memory.\nPlay a map.");
	}
}
#endif