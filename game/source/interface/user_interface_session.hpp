#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

enum e_controller_index;
struct s_player_configuration;
struct s_player_identifier;
struct c_map_variant;
struct c_game_variant;

extern bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode);
extern e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, dword* player_error_mask);
extern e_controller_index user_interface_session_get_controller_index(long session_player_index);
extern s_player_configuration* __cdecl user_interface_session_get_player_data(long session_player_index);
extern bool __cdecl user_interface_session_is_local_player(long session_player_index);
extern bool __cdecl user_interface_squad_is_player_valid(long session_player_index);
extern long __cdecl user_interface_squad_get_player_index(s_player_identifier const* player_identifier);
extern long __cdecl user_interface_squad_get_countdown_delaying_player();
extern long __cdecl user_interface_squad_get_countdown_timer();
extern e_gui_game_mode __cdecl user_interface_squad_get_ui_game_mode();
extern bool __cdecl user_interface_squad_set_map_variant(c_map_variant const* map_variant);
extern bool __cdecl user_interface_squad_set_game_variant(c_game_variant const* game_variant);

