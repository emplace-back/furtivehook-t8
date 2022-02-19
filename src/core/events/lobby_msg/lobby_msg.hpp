#pragma once
#include "dependencies/std_include.hpp"

namespace events::lobby_msg
{
	using callback = std::function<bool(const game::netadr_t&, game::msg_t&)>;

	size_t handle_packet_stub();
	std::string build_lobby_msg(const game::LobbyModule module);
	void on_message(const game::LobbyModule& lobby_module, const callback& callback);
	void initialize();

	extern bool log_messages;
}