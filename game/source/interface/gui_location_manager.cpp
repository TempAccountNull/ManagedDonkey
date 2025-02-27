#include "interface/gui_location_manager.hpp"

#include "cseries/cseries_console.hpp"
#include "interface/user_interface_memory.hpp"
#include "interface/user_interface_messages.hpp"
#include "memory/module.hpp"
#include "tag_files/string_ids.hpp"


HOOK_DECLARE_CLASS(0x00ADF6E0, c_gui_location_manager, change_location);
HOOK_DECLARE_CLASS(0x00ADF870, c_gui_location_manager, get_current_ui_location);
HOOK_DECLARE(0x00ADF9D0, location_manager_get);
HOOK_DECLARE(0x00ADF9E0, location_manager_start);
HOOK_DECLARE(0x00ADF9F0, location_manager_stop);

REFERENCE_DECLARE(0x01944204, c_gui_location_manager, g_location_manager);

// 00ADF680
c_gui_location_manager::c_gui_location_manager() :
	m_running(true),
	__unknown4(0),
	m_show_postgame_stats_upon_lobby_entrance(false),
	m_change_time(0xFFFFFFFF),
	m_change_location(30)
{
}

// 00ADF6C0
bool __fastcall c_gui_location_manager::can_change_location(c_gui_location_manager* _this)
{
	return _this->m_change_location >= 30;
}

// 00ADF6E0
void __fastcall c_gui_location_manager::change_location(c_gui_location_manager* _this, long gui_location, long screen_name)
{
	bool can_change_location = _this->can_change_location(_this);

	HOOK_INVOKE_CLASS(, c_gui_location_manager, change_location, void(__thiscall*)(c_gui_location_manager*, long), _this, screen_name);

	if (can_change_location)
	{
		c_load_screen_message* message = (c_load_screen_message*)user_interface_malloc_tracked(sizeof(c_load_screen_message), __FILE__, __LINE__);
		if (load_screen_message_ctor(message, screen_name, k_any_controller, _window_index4, STRING_ID(gui, bottom_most)))
			user_interface_messaging_post(message);
	}
}

// 00ADF870
long __fastcall c_gui_location_manager::get_current_ui_location(c_gui_location_manager* _this)
{
	long result = 0;
	HOOK_INVOKE_CLASS(result =, c_gui_location_manager, get_current_ui_location, long(__thiscall*)(c_gui_location_manager*), _this);
	return result;
}

// 00ADF8A0
c_gui_screen_widget* __fastcall c_gui_location_manager::get_location_screen(c_gui_location_manager* _this)
{
	c_gui_screen_widget* result = DECLFUNC(0x00ADF8A0, c_gui_screen_widget*, __thiscall, c_gui_location_manager*)(_this);

	return result;
}

// 00ADF8F0
long __fastcall c_gui_location_manager::get_location_screen_name(c_gui_location_manager* _this, long gui_location)
{
	long result = DECLFUNC(0x00ADF8F0, long, __thiscall, c_gui_location_manager*, long)(_this, gui_location);

	return result;
}

// 00ADF960
long __fastcall c_gui_location_manager::get_pregame_lobby_name(c_gui_location_manager* _this, long gui_game_mode)
{
	long result = DECLFUNC(0x00ADF960, long, __thiscall, c_gui_location_manager*, long)(_this, gui_game_mode);

	return result;
}

// 00ADF9D0
c_gui_location_manager* __cdecl location_manager_get()
{
	return &g_location_manager;
}

// 00ADF9E0
void __cdecl location_manager_start()
{
	g_location_manager.m_running = true;
}

// 00ADF9F0
void __cdecl location_manager_stop()
{
	g_location_manager.m_running = false;
}

// 00ADFA00
void __fastcall c_gui_location_manager::set_running(c_gui_location_manager* _this, bool running)
{
	_this->m_running = running;
}

// 00ADFA10
void __fastcall c_gui_location_manager::update(c_gui_location_manager* _this)
{
	DECLFUNC(0x00ADFA10, void, __thiscall, c_gui_location_manager*)(_this);
}

void __cdecl window_manager_load_screen_hs(long screen_name)
{
	INVOKE(0x00AAD9A0, window_manager_load_screen_hs, screen_name);
}