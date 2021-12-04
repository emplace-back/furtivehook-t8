#include "dependencies/std_include.hpp"
#include "instant_message.hpp"

namespace events::instant_message
{
	constexpr auto nonce{ 1111111111u };
	
	void send_info_request(const std::vector<std::uint64_t>& recipients)
	{
		char buf[0x20000] = { 0 };
		game::LobbyMsg lobby_msg{};

		game::LobbyMsgRW_PrepWriteMsg(&lobby_msg, buf, sizeof buf, game::MESSAGE_TYPE_INFO_REQUEST, 0);
		game::LobbyMsgRW_PackageUInt(&lobby_msg, "nonce", &nonce);

		game::dwInstantSendMessage(0, recipients.data(), recipients.size(), 'h', lobby_msg.msg.data, lobby_msg.msg.cursize);
	}

	void add_friend_response(friends::friends_t& friends, const response_t& response)
	{
		if (const auto is_valid{ friends.response.valid == response.valid }; is_valid)
		{
			friends.response = response;
			return;
		}

		friends.response = response;
		friends.last_online = response.last_online;
		friends::write_to_friends();

		PRINT_MESSAGE("%s is online.", friends.name.data());
	}
	
	void add_friend_response(const std::uint64_t sender_id, game::Msg_InfoResponse& response)
	{
		const auto lobby{ response.lobby[0] };
		const auto last_online{ std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() };

		for (auto& friends : friends::friends)
		{
			if (sender_id == friends.id)
			{
				add_friend_response(friends, { true, lobby, last_online });
			}
		}
	}
	
	namespace dispatch
	{
		std::unordered_map<std::uint8_t, callback> messages;
		bool log_messages = true; 
		
		void on_message(const std::uint8_t command, const callback& callback)
		{
			if (messages.find(command) == messages.end())
			{
				messages[command] = callback;
			}
		}

		void log_instant_messages(const std::uint64_t sender_id, const std::uint32_t message_size, const std::uint8_t type)
		{
			const auto message{ "Received instant message '%c' of size [%u] from (%llu)" };
			
			if (!log_messages)
			{
				return logger::print_log(message, type, message_size, sender_id);
			}

			PRINT_LOG(message, type, message_size, sender_id);
		}
		
		bool handle_message(const std::uint64_t sender_id, const char* message, const std::uint32_t message_size)
		{
			if (message_size >= 0x1000)
			{
				PRINT_LOG("Received instant message of invalid size [%u] from (%llu)", message_size, sender_id);
				return true;
			}

			game::msg_t msg{};
			game::MSG_InitReadOnly(&msg, message, message_size);
			game::MSG_BeginReading(&msg);

			auto type{ 0ui8 };

			if (game::MSG_ReadByte(&msg) == '2')
			{
				type = game::MSG_ReadByte(&msg);
			}
			
			if (const auto message_func{ messages.find(type) }; message_func != messages.end())
			{
				const auto msg_backup{ msg }; 
				const auto cb{ message_func->second(sender_id, msg) };

				if (msg.readcount != msg_backup.readcount)
					msg = msg_backup;

				return cb;
			}

			instant_message::dispatch::log_instant_messages(sender_id, message_size, type); 
			return false;
		}
		
		void initialize()
		{
			exception::hbp::register_exception(game::base_address + 0x2ED0F84, [](const LPEXCEPTION_POINTERS ex)
			{
				ex->ContextRecord->Rsp -= 0x870;

				const auto sender_id{ ex->ContextRecord->Rcx };
				const auto message{ reinterpret_cast<const char*>(ex->ContextRecord->R8) };
				const auto message_size{ ex->ContextRecord->R9 };

				if (events::instant_message::dispatch::handle_message(sender_id, message, message_size))
				{
					ex->ContextRecord->Rcx = *reinterpret_cast<std::uintptr_t*>(ex->ContextRecord->Rsp + 0x860); 
					ex->ContextRecord->Rip = game::base_address + 0x2ED10E5;
					return EXCEPTION_CONTINUE_EXECUTION;
				}

				ex->ContextRecord->Rip += 7;
				return EXCEPTION_CONTINUE_EXECUTION;
			}); 

			instant_message::dispatch::on_message('h', [=](const auto& sender_id, auto& msg)
			{
				const auto size{ msg.cursize - msg.readcount };

				char data[2048] = { 0 };
				game::MSG_ReadData(&msg, data, sizeof data, size);

				if (!msg.overflowed)
				{
					game::LobbyMsg lobby_msg{};

					if (!game::LobbyMsgRW_PrepReadData(&lobby_msg, data, size))
						return false;

					if (lobby_msg.msg.cursize == lobby_msg.msg.readcount)
					{
						PRINT_MESSAGE("Received invalid message from (%llu)", sender_id);
						return true;
					}

					if (lobby_msg.type == game::MESSAGE_TYPE_INFO_REQUEST)
					{
						PRINT_LOG("Received a join request from (%llu)", sender_id);
					}
					else if (lobby_msg.type == game::MESSAGE_TYPE_INFO_RESPONSE)
					{
						game::Msg_InfoResponse response{};

						if (!game::MSG_InfoResponse(&response, &lobby_msg))
							return false;

						if (response.nonce == nonce)
						{
							add_friend_response(sender_id, response);
						}
						else
						{
							PRINT_LOG("Received join response from (%llu) %s",
								sender_id,
								utils::string::adr_to_string(&response.lobby[0].serializedAdr.xnaddr).data());
						}
					}
				}

				return false;
			});
		}
	}

	void initialize()
	{
		instant_message::dispatch::initialize();

		scheduler::on_dw_initialized([]()
		{
			for (const auto& friends : friends::friends)
			{
				std::vector<std::uint64_t> recipients;
				recipients.emplace_back(friends.id);

				if (game::LiveUser_IsXUIDLocalPlayer(friends.id) || recipients.size() > 18)
					continue;
				
				send_info_request(recipients);
			}

		}, scheduler::pipeline::main, 10s);
	}
}