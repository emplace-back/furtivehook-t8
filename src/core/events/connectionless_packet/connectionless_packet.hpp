#pragma once
#include "dependencies/std_include.hpp"

namespace events::connectionless_packet
{
	using callback = std::function<bool(const command::args_&, const game::netadr_t&, game::msg_t&)>;
	
	size_t handle_command_stub();
	void on_command(const std::string& command, const callback& callback);
	
	extern bool log_commands;
}