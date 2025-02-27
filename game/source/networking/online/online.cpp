#include "networking/online/online.hpp"

#include "cseries/cseries_console.hpp"
#include "memory/module.hpp"
#include "text/unicode.hpp"

#include <winsock.h>

HOOK_DECLARE(0x004429C0, online_dispose);
//HOOK_DECLARE(0x004429D0, online_dump_machine_info);
//HOOK_DECLARE(0x004429F0, online_get_maximum_compatible_nat_type);
//HOOK_DECLARE(0x00442A00, online_get_nat_type);
//HOOK_DECLARE(0x00442A20, online_get_title_name_string);
//HOOK_DECLARE(0x00442A50, online_has_all_online_enabled_users);
//HOOK_DECLARE(0x00442A60, online_has_any_silver_or_gold_live_users);
HOOK_DECLARE(0x00442A70, online_initialize);
//HOOK_DECLARE(0x00442A90, online_is_connected_to_live);
HOOK_DECLARE(0x00442AA0, online_user_get_name);
//HOOK_DECLARE(0x00442AB0, online_user_get_player_identifier);
//HOOK_DECLARE(0x00442AE0, online_user_get_xuid);
//HOOK_DECLARE(0x00442B00, sub_442B00);
//HOOK_DECLARE(0x00442B20, sub_442B20);
//HOOK_DECLARE(0x00442B40, online_has_signed_in_user);
////HOOK_DECLARE(0x00442B60, online_local_xuid_is_silver_or_gold_live); // crashes
//HOOK_DECLARE(0x00442B70, sub_442B70);
//HOOK_DECLARE(0x00442B80, online_process_debug_output_queue);
//HOOK_DECLARE(0x00442B90, online_user_set_xuid);
//HOOK_DECLARE(0x00442BB0, online_set_is_connected_to_live);
HOOK_DECLARE(0x00442BC0, online_user_set_name);
HOOK_DECLARE(0x00442BF0, online_update);

REFERENCE_DECLARE(0x019AB728, s_online_user, g_online_user);
REFERENCE_DECLARE(0x02179468, bool, g_online_is_connected_to_live);

static s_online_user g_controller_users[4] =
{
	g_online_user,
	{.initialized = true, .player_xuid = 1, .player_name = L"user1" },
	{.initialized = true, .player_xuid = 2, .player_name = L"user2" },
	{.initialized = true, .player_xuid = 3, .player_name = L"user3" },
};

long g_nat_type_override = _online_nat_type_none;
char g_hostname[264];

void __cdecl online_dispose()
{
}

void __cdecl online_dump_machine_info(qword game_instance)
{
	c_console::write_line("networking:online:machine_info: game instance-%0I64X", game_instance);
}

e_online_nat_type __cdecl online_get_maximum_compatible_nat_type(e_online_nat_type nat_type)
{
	if (nat_type >= k_online_nat_type_count)
	{
		c_console::write_line("online: unexpected nat type %d passed to online_get_maximum_compatible_nat_type()", nat_type);
		return _online_nat_type_open;
	}

	switch (nat_type)
	{
	case _online_nat_type_none:
		return _online_nat_type_none;
	case _online_nat_type_open:
		return _online_nat_type_open;
	case _online_nat_type_moderate:
		return _online_nat_type_moderate;
	case _online_nat_type_strict:
		return _online_nat_type_strict;
	}

	return _online_nat_type_none;
}

e_online_nat_type __cdecl online_get_nat_type()
{
	long result = _online_nat_type_none;
	//XONLINE_NAT_TYPE xnat_type XOnlineGetNatType()

	if (!online_is_connected_to_live() /* || xnat_type == XONLINE_NAT_OPEN*/)
	{
		result = _online_nat_type_open;
	}
	//else if (xnat_type == XONLINE_NAT_MODERATE)
	//{
	//	result = _online_nat_type_moderate;
	//}
	//else if (xnat_type == XONLINE_NAT_STRICT)
	//{
	//	result = _online_nat_type_strict;
	//}
	//else
	//{
	//	c_console::write_line("online: XOnlineGetNatType returned unknown NAT type 0x%08lX", xnat_type);
	//}

	if (g_nat_type_override)
	{
		if (g_nat_type_override < _online_nat_type_open || g_nat_type_override >= k_online_nat_type_count)
		{
			c_console::write_line("online: invalid NAT override %d, resetting", g_nat_type_override);
			g_nat_type_override = _online_nat_type_none;
		}
		else
		{
			result = g_nat_type_override;
		}
	}

	return static_cast<e_online_nat_type>(result);
}

void __cdecl online_get_title_name_string(dword title_id, wchar_t* buffer, long buffer_length)
{
	usnzprintf(buffer, buffer_length, L"%08lX", title_id);
}

bool __cdecl online_has_all_online_enabled_users()
{
	return true;
}

bool __cdecl online_has_any_silver_or_gold_live_users()
{
	return true;
}

void __cdecl online_initialize()
{
	//online_set_is_connected_to_live(true);
	gethostname(g_hostname, 256);
}

bool __cdecl online_is_connected_to_live()
{
	return g_online_is_connected_to_live;
}

wchar_t const* __cdecl online_user_get_name(long controller_index)
{
	return g_controller_users[controller_index].player_name;
}

qword __cdecl online_user_get_player_identifier(long controller_index)
{
	qword result = 0;
	//HOOK_INVOKE(result =, online_user_get_player_identifier, controller_index);
	return result;
}

qword __cdecl online_user_get_xuid(long controller_index)
{
	return g_controller_users[controller_index].player_xuid;
}

bool __cdecl sub_442B00(long controller_index)
{
	return controller_index == 0;
}

bool __cdecl sub_442B20()
{
	return true;
}

bool __cdecl online_has_signed_in_user(long controller_index)
{
	return g_controller_users[controller_index].initialized;
}

bool online_local_xuid_is_silver_or_gold_live(long controller_index)
{
	return g_controller_users[controller_index].initialized;
}

bool __cdecl sub_442B70()
{
	return true;
}

void __cdecl online_process_debug_output_queue()
{
}

void __cdecl online_user_set_xuid(qword xuid)
{
	g_controller_users[0].player_xuid = xuid;
}

void __cdecl online_set_is_connected_to_live(bool is_connected_to_live)
{
	g_online_is_connected_to_live = is_connected_to_live;
}

void __cdecl online_user_set_name(wchar_t const* name)
{
	ustrnzcpy(g_controller_users[0].player_name, name, 16);
}

void __cdecl online_update()
{
	if (*g_controller_users[0].player_name == 0)
	{
		wsprintfW(g_controller_users[0].player_name, L"%S", g_hostname);
	}
}
