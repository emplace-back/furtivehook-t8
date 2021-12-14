#include "dependencies/std_include.hpp"
#include "security.hpp"

namespace security
{
	bool handle_connection_response_migration(const command::args_&, const game::netadr_t& from, game::msg_t&)
	{
		PRINT_MESSAGE("Kick attempt caught from %s", utils::string::adr_to_string(&from).data());
		return true;
	}
	
	bool handle_mstart(const command::args_&, const game::netadr_t& from, game::msg_t&)
	{
		PRINT_MESSAGE("Migration screen attempt caught from %s", utils::string::adr_to_string(&from).data());
		return true;
	}
	
	bool handle_friend_message(game::msg_t& msg, const std::uint64_t sender_id)
	{
		game::JoinSessionMessage message{};

		if (msg.cursize - msg.readcount == sizeof message)
		{
			game::MSG_ReadData(&msg, &message, sizeof message, sizeof message);

			if (message.mType == game::JOIN_REQUEST)
			{
				PRINT_MESSAGE("Join request attempt prevented from (%llu)", sender_id);
				return true;
			}
			else if (message.mType == game::JOIN_REPLY)
			{
				PRINT_MESSAGE("Popup attempt caught from (%llu)", sender_id);
				return true;
			}

			return false;
		}

		PRINT_MESSAGE("Popup attempt caught from (%llu)", sender_id);
		return true;
	}

	bool handle_remote_command_message(game::msg_t&, const std::uint64_t sender_id)
	{
		PRINT_MESSAGE("Remote command attempt caught from (%llu)", sender_id);
		return true;
	}
	
	void initialize()
	{
		security::iat::initialize(); 

		events::instant_message::on_message('f', &handle_friend_message);
		events::instant_message::on_message('e', &handle_remote_command_message);

		events::connectionless_packet::on_command("mstart", &handle_mstart);
		events::connectionless_packet::on_command("connectResponseMigration", &handle_connection_response_migration);
	}
}