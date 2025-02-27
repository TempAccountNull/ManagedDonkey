#include "game/multiplayer_definitions.hpp"

#include "cache/cache_files.hpp"
#include "game/game_globals.hpp"
#include "scenario/scenario.hpp"

long __cdecl multiplayer_universal_data_get_random_weapon_definition_index()
{
    return INVOKE(0x0069CC30, multiplayer_universal_data_get_random_weapon_definition_index);
}

long __cdecl multiplayer_universal_data_get_remapped_vehicle_definition_index(long vehicle_tag_index, s_multiplayer_vehicle_set const* vehicle_set)
{
    return INVOKE(0x0069CDC0, multiplayer_universal_data_get_remapped_vehicle_definition_index, vehicle_tag_index, vehicle_set);
}

long __cdecl multiplayer_universal_data_get_remapped_weapon_definition_index(long weapon_tag_index, s_multiplayer_weapon_set const* weapon_set)
{
    return INVOKE(0x0069CE90, multiplayer_universal_data_get_remapped_weapon_definition_index, weapon_tag_index, weapon_set);
}

long __cdecl multiplayer_universal_data_get_vehicle_selection_definition_index(long name)
{
    return INVOKE(0x0069CF70, multiplayer_universal_data_get_vehicle_selection_definition_index, name);
}

short __cdecl multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id(long name)
{
    return INVOKE(0x0069CFF0, multiplayer_universal_data_get_vehicle_set_absolute_index_from_string_id, name);
}

long __cdecl multiplayer_universal_data_get_vehicle_set_name_from_absolute_index(short absolute_index)
{
    return INVOKE(0x0069D070, multiplayer_universal_data_get_vehicle_set_name_from_absolute_index, absolute_index);
}

long __cdecl multiplayer_universal_data_get_weapon_selection_definition_index(long name)
{
    return INVOKE(0x0069D0E0, multiplayer_universal_data_get_weapon_selection_definition_index, name);
}

short __cdecl multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id(long name)
{
    return INVOKE(0x0069D160, multiplayer_universal_data_get_weapon_set_absolute_index_from_string_id, name);
}

long __cdecl multiplayer_universal_data_get_weapon_set_name_from_absolute_index(short absolute_index)
{
    return INVOKE(0x0069D1D0, multiplayer_universal_data_get_weapon_set_name_from_absolute_index, absolute_index);
}

s_multiplayer_vehicle_set const* __cdecl multiplayer_universal_data_vehicle_set_try_and_get(short vehicle_set_index)
{
    return INVOKE(0x0069D250, multiplayer_universal_data_vehicle_set_try_and_get, vehicle_set_index);
}

s_multiplayer_weapon_set const* __cdecl multiplayer_universal_data_weapon_set_try_and_get(short weapon_set_index)
{
    return INVOKE(0x0069D2B0, multiplayer_universal_data_weapon_set_try_and_get, weapon_set_index);
}

s_multiplayer_runtime_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_runtime_data()
{
    return INVOKE(0x0069D310, scenario_multiplayer_globals_try_and_get_runtime_data);
}

s_multiplayer_universal_globals_definition* __cdecl scenario_multiplayer_globals_try_and_get_universal_data()
{
    s_game_globals* game_globals = scenario_try_and_get_game_globals();

    s_multiplayer_globals_definition* multiplayer_globals = nullptr;
    if (game_globals)
        multiplayer_globals = static_cast<s_multiplayer_globals_definition*>(tag_get('mulg', game_globals->multiplayer_globals.index));

    s_multiplayer_universal_globals_definition* universal_globals = nullptr;
    if (multiplayer_globals)
        universal_globals = multiplayer_globals_try_and_get_universal_data(multiplayer_globals);

    return universal_globals;

    return INVOKE(0x0069D340, scenario_multiplayer_globals_try_and_get_universal_data);
}

s_multiplayer_universal_globals_definition* multiplayer_globals_try_and_get_universal_data(s_multiplayer_globals_definition* multiplayer_globals)
{
    if (multiplayer_globals && multiplayer_globals->universal.count() > 0)
        return multiplayer_globals->universal.begin();

    return nullptr;
}

