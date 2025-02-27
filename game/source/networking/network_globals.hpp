#pragma once

struct s_network_globals
{
	bool initialized;
	bool entered;

	// halo 3: thread_id = system_get_current_thread_id()
	// halo reach: thread_index = get_current_thread_index()
	// halo online: unused
	unsigned long thread_id;
};
extern s_network_globals& network_globals;

struct c_network_session_manager;
struct c_map_variant;
struct c_game_variant;
enum e_game_engine_type;

extern void __cdecl network_dispose();
extern c_network_session_manager* __cdecl network_get_session_manager();
extern void __cdecl network_initialize();
extern bool __cdecl network_initialized();
extern void __cdecl network_receive();
extern void __cdecl network_send();
extern void __cdecl network_set_online_environment(bool online_environment);
extern void __cdecl network_shutdown_transport(void* userdata);
extern void __cdecl network_startup_transport(void* userdata);
extern void __cdecl network_update();
extern void __cdecl network_test_set_map_name(char const* scenario_path);
extern void __cdecl network_test_set_map_variant(c_map_variant const* map_variant);
extern void __cdecl network_test_set_game_variant(char const* game_engine_name);
extern void __cdecl network_test_set_session_mode(char const* session_mode_name);
extern void __cdecl network_test_set_ui_game_mode(char const* ui_game_mode_name);
extern void __cdecl network_test_set_advertisement_mode(char const* advertisement_mode_name);
extern void __cdecl network_test_set_game_variant_parameter(char const* parameter_name, long value, long* old_value);

