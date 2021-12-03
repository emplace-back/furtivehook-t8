#pragma once
#include "dependencies/std_include.hpp"

namespace events::instant_message
{
	struct response_t
	{
		bool valid;
		game::InfoResponseLobby lobby;
		std::int64_t last_online;
	}; 
	
	namespace dispatch
	{
		using callback = std::function<bool(const std::uint64_t, game::msg_t&)>;
		void on_message(const std::uint8_t command, const callback& callback);

		extern bool log_messages;
	}

	void send_info_request(const std::vector<std::uint64_t>& recipients);
	void initialize();
}