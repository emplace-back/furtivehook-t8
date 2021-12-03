#include "dependencies/std_include.hpp"
#include "security.hpp"

namespace security
{
	void initialize()
	{
		for (const auto& handler : game::handlers)
		{
			events::connectionless_packet::on_command(handler.first, [=](const auto& args, const auto& target, const auto&)
			{
				PRINT_LOG("Ignoring OOB '%s' from %s", args.join().data(), utils::string::adr_to_string(&target).data());
				return true;
			});
		}
		
		events::instant_message::dispatch::on_message('f', [=](const auto& sender_id, auto& msg)
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
		});
		
		events::instant_message::dispatch::on_message('e', [=](const auto& sender_id, const auto&)
		{
			PRINT_LOG("Ignoring remote command instant message from (%llu)", sender_id);
			return true;
		});
	}
}