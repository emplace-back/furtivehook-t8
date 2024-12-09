#include "dependencies/std_include.hpp"
#include "instant_message.hpp"

namespace events::instant_message
{
	bool log_messages = false;

	namespace
	{
		bool handle_lobby_message(game::msg_t& msg, const std::uint64_t sender_id)
		{
			const auto length{ msg.cursize - msg.readcount };

			char buffer[2048] = { 0 };
			game::MSG_ReadData(&msg, buffer, sizeof buffer, length);

			if (msg.overflowed)
				return true;

			if (!game::LobbyMsgRW_PrepReadData(&msg, buffer, length))
				return true;

			if (msg.type == game::MESSAGE_TYPE_INFO_REQUEST)
			{
				PRINT_LOG("Received a join request from (%llu)", sender_id); 
				return true;
			}
			else if (msg.type == game::MESSAGE_TYPE_INFO_RESPONSE)
			{
				game::Msg_InfoResponse response{};

				if (!game::MSG_InfoResponse(&response, &msg))
					return true;

				if (response.nonce != friends::NONCE)
				{
					PRINT_LOG("Received join response from (%llu) %s",
						sender_id,
						utils::string::adr_to_string(&response.lobby[0].serializedAdr.xnaddr).data());

					return false;
				}

				friends::add_friend_response(response, sender_id);
				//return true;
			}

			return false;
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
		
		std::unordered_map<std::uint8_t, callback>& get_callbacks()
		{
			static std::unordered_map<std::uint8_t, callback> callbacks{};
			return callbacks;
		}

		bool __fastcall dispatch_message(game::msg_t* msg, std::uint8_t type, std::uint64_t sender_id)
		{
			const auto& callbacks = get_callbacks();
			const auto handler = callbacks.find(type);

			if (handler == callbacks.end())
			{
				log_instant_messages(sender_id, msg->cursize, type);
				return false;
			}

			const auto msg_backup = *msg;
			const auto callback = handler->second(*msg, sender_id);

			if (msg->readcount != msg_backup.readcount)
				*msg = msg_backup;

			return callback;
		}
	}

	size_t dispatch_message_stub()
	{
		const static auto stub = utils::hook::assemble([](utils::hook::assembler& a)
		{
			const auto return_original = a.newLabel();

			a.mov(r9, rcx); // message
			a.movzx(ebx, al);

			a.pushad64();
			a.mov(r8, rsi); // sender_id
			a.mov(rdx, ebx); // type
			a.mov(rcx, r9); // message
			a.call_aligned(instant_message::dispatch_message);
			a.test(al, al);
			a.jz(return_original);
			a.popad64();

			a.xor_(al, al);
			a.jmp(game::base_address + 0x2ED10D3);

			a.bind(return_original);
			a.popad64();
			a.jmp(game::base_address + 0x2ED0FE5);
		});

		return reinterpret_cast<size_t>(stub);
	}
	
	void send_info_request(const std::vector<std::uint64_t>& recipients)
	{
		if (game::Live_IsDemonwareFetchingDone(0))
		{
			char buf[0x20000] = { 0 };
			game::msg_t msg{};

			game::LobbyMsgRW_PrepWriteMsg(&msg, buf, sizeof buf, game::MESSAGE_TYPE_INFO_REQUEST, 0);
			game::LobbyMsgRW_PackageUInt(&msg, "nonce", &friends::NONCE);

			game::send_instant_message(recipients, 'h', msg.data, msg.cursize);
		}
	}

	void on_message(const std::uint8_t type, const callback& callback)
	{
		get_callbacks()[type] = callback;
	}
	
	void initialize()
	{
		instant_message::on_message('h', &handle_lobby_message);
	}
}