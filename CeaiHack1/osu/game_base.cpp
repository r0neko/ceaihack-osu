#include "game_base.h"
#include "../ch_memory_addy.h"

using namespace ceaihack::osu;

gamebase* gamebase::get_instance() {
	return new gamebase();
}

int gamebase::get_time() {
	return *(int*) ceaihack::memory::location::time_base;
}

int gamebase::get_game_time() {
	return *(int*)ceaihack::memory::location::gb_time_base;
}