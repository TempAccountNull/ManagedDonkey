#include "main_game_launch.hpp"

#include "cseries/cseries_console.hpp"
#include "game/game.hpp"
#include "main/main_game.hpp"
#include "networking/logic/network_life_cycle.hpp"


s_main_game_launch_globals g_launch_globals = { .player_count = 1, .options = *reinterpret_cast<game_options*>(0x023B6348) };

// void main_game_launch_default()
// void main_game_launch_default_editor()

void main_game_launch_initialize()
{
	//game_options_new(&g_launch_globals.options);
}

void main_game_launch_set_map_name(char const* map_name)
{
	ASSERT(map_name);
	g_launch_globals.options.scenario_path.set(map_name);
}

void main_game_launch_set_difficulty(e_campaign_difficulty_level difficulty)
{
	g_launch_globals.options.game_mode = _game_mode_campaign;
	g_launch_globals.options.campaign_difficulty = difficulty;
	game_set_difficulty(difficulty);
}

void main_game_launch_campaign()
{
	g_launch_globals.options.game_mode = _game_mode_campaign;
}

void main_game_launch_survival()
{
	g_launch_globals.options.game_mode = _game_mode_campaign;
	g_launch_globals.options.survival_enabled = true;
}

void main_game_launch_multiplayer()
{
	g_launch_globals.options.game_mode = _game_mode_multiplayer;
}

void main_game_launch_ui_shell()
{
	g_launch_globals.options.game_mode = _game_mode_mainmenu;
}

void main_game_launch_set_coop_player_count(long coop_player_count)
{
	if (coop_player_count > 0 && coop_player_count <= 4)
	{
		g_launch_globals.options.game_mode = _game_mode_campaign;
		g_launch_globals.player_count = coop_player_count;
	}
	else
	{
		c_console::write_line("main_game_launch_set_coop_player_count: invalid player count %d (must be from 1-%d)", coop_player_count, 4);
	}
}

void main_game_launch_set_multiplayer_splitscreen_count(long multiplayer_splitscreen_count)
{
	if (multiplayer_splitscreen_count > 0 && multiplayer_splitscreen_count <= 4)
	{
		g_launch_globals.options.game_mode = _game_mode_multiplayer;
		g_launch_globals.player_count = multiplayer_splitscreen_count;
		if (g_launch_globals.options.game_variant.get_game_engine_index() == _game_engine_base_variant)
			g_launch_globals.options.game_variant.set_game_engine_index(_game_engine_slayer_variant);

		//if (g_launch_globals.options.game_variant.m_game_engine_index == _game_engine_base_variant)
		//    build_default_game_variant(&g_launch_globals.options.game_variant, _game_engine_slayer_variant);
	}
	else
	{
		c_console::write_line("main_game_launch_set_multiplayer_splitscreen_count: invalid player count %d (must be from 1-%d)", multiplayer_splitscreen_count, 4);
	}
}

void main_game_launch_set_multiplayer_engine(char const* multiplayer_engine)
{
	e_game_engine_type game_engine_index;

	for (long i = _game_engine_base_variant; i < k_game_engine_type_count; i++)
	{
		if (csstricmp(multiplayer_engine, game_engine_variant_get_name(i)) != 0)
			continue;

		game_engine_index = e_game_engine_type(i);
	}

	g_launch_globals.options.game_variant.set_game_engine_index(game_engine_index);
}

// void main_game_launch_set_multiplayer_variant(char const* multiplayer_variant)

void main_game_launch_set_initial_zone_set_index(long initial_zone_set_index)
{
	g_launch_globals.options.initial_zone_set_index = static_cast<short>(initial_zone_set_index);
}

void main_game_launch_set_insertion_point(short insertion_point)
{
	g_launch_globals.options.campaign_insertion_point = insertion_point;
}

// void main_game_launch_set_tick_rate(long tick_rate)
// void main_game_launch_legacy(char const* map_name)

void main_game_launch(const char* map_name)
{
	main_game_launch_set_map_name(map_name);
	if (network_life_cycle_get_state())
		network_life_cycle_end();

	if (g_launch_globals.options.game_mode == _game_mode_campaign)
	{
		if (g_launch_globals.options.campaign_difficulty.get() <= _campaign_difficulty_level_easy)
			g_launch_globals.options.campaign_difficulty = _campaign_difficulty_level_easy;
		if (g_launch_globals.options.campaign_difficulty.get() > k_campaign_difficulty_levels_count)
			g_launch_globals.options.campaign_difficulty = _campaign_difficulty_level_legendary;
	}
	else if (g_launch_globals.options.game_mode != _game_mode_multiplayer)
	{
		c_console::write_line("main_game_launch: unknown game mode %d!", g_launch_globals.options.game_mode.get());
		return;
	}

	g_launch_globals.options.record_saved_film = false;

	if (g_launch_globals.player_count < 1)
		g_launch_globals.player_count = 1;
	if (g_launch_globals.player_count > 4)
		g_launch_globals.player_count = 4;

	game_options_setup_default_players(g_launch_globals.player_count, &g_launch_globals.options);
	//game_options_validate(&g_launch_globals.options);
	main_game_change(&g_launch_globals.options);
}

// void main_game_launch_process()

void main_game_launch_set_active_skulls_primary(long active_skulls_primary)
{
	g_launch_globals.options.skulls_primary = active_skulls_primary;
}

void main_game_launch_set_active_skulls_secondary(long active_skulls_secondary)
{
	g_launch_globals.options.skulls_secondary = active_skulls_secondary;
}

long main_game_launch_get_player_count()
{
	return g_launch_globals.player_count;
}

// the last player controls the game visuals, hud and other stuff
long main_game_launch_get_last_player()
{
	return g_launch_globals.player_count - 1;
}
