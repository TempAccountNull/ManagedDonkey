#include "life_cycle_manager.hpp"

#include "cseries/cseries_console.hpp"

#include <string>

void c_life_cycle_state_manager::request_state_change(e_life_cycle_state state, long entry_data_size, void* entry_data)
{
	ASSERT(entry_data_size <= k_maximum_state_change_entry_data_size);
	ASSERT(state != m_current_state);

	m_pending_state_change = true;
	m_pending_state = state;
	m_entry_data_size = entry_data_size;
	memset(m_entry_data, 0, k_maximum_state_change_entry_data_size);

	if (m_entry_data_size > 0)
	{
		ASSERT(entry_data != NULL);
		memcpy(m_entry_data, entry_data, m_entry_data_size);
	}
}

void c_life_cycle_state_manager::request_leave_sessions(bool disconnect)
{
	c_console::write_line("networking:logic:life-cycle: leave requested to life-cycle manager (disconnect %s)", disconnect ? "TRUE" : "FALSE");

	if (m_current_state == _life_cycle_state_leaving)
		c_console::write_line("networking:logic:life-cycle: we are already leaving, no need to try again");
	else
		request_state_change(_life_cycle_state_leaving, 1, &disconnect);
}

void c_life_cycle_state_manager::set_current_state(e_life_cycle_state state, long entry_data_size, void* entry_data)
{
	ASSERT((entry_data_size == 0 && entry_data == NULL) || (entry_data_size > 0 && entry_data != NULL));
	if (m_current_state != state)
	{
		ASSERT(state >= 0 && state < k_life_cycle_state_count);

		c_life_cycle_state_handler* from_handler = m_handlers[get_current_state()];
		c_life_cycle_state_handler* to_handler = m_handlers[state];
		ASSERT(from_handler != NULL);
		ASSERT(to_handler != NULL);

		from_handler->exit(to_handler);
		to_handler->enter(from_handler, entry_data_size, entry_data);
	}
}

e_life_cycle_state c_life_cycle_state_manager::get_current_state()
{
	ASSERT(m_current_state >= _life_cycle_state_none && m_current_state < k_life_cycle_state_count);
	return m_current_state;
}

void c_life_cycle_state_manager::terminate()
{
	set_current_state(_life_cycle_state_none, 0, nullptr);
}

