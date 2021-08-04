#include "spdlog/spdlog.h"

#include "ch.h"
#include "ch_hooks.h"
#include "ch_config.h"
#include "ch_memory.h"
#include "ch_auth.h"
#include "ch_logger.h"
#include "ch_clock.h"

#include "./reflection/ch_reflection.h"
#include "./osu/osu.h"

#include "ch_game_modifier.h"

DWORD WINAPI ceaihack::cheat::init(LPVOID lpParam) {
	ceaihack::logger::init();
	ceaihack::reflection::init();

	/*while (i < 60) {
		ceaihack::osu::player::get_instance();
		Sleep(100);
	}*/

	ceaihack::cheat::memory::init();
	ceaihack::hooks::init();
	ceaihack::cheat::auth::init();

	while (ceaihack::config::is_running) {
		/*ceaihack::cheat::features::game_modifier::update();
		Sleep(15);*/
	}

	ceaihack::hooks::unload();

	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);

	return 0;
}