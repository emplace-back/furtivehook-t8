#include "dependencies/std_include.hpp"
#include "lobby_msg.hpp"

namespace events::lobby_msg
{
	std::unordered_map<game::LobbyModule, callback> messages;
	bool log_messages = false;

	void on_message(const game::LobbyModule& lobby_module, const callback& callback)
	{
		if (messages.find(lobby_module) == messages.end())
		{
			messages[lobby_module] = callback;
		}
	}
	
	void log_lobby_messages(const game::netadr_t* from, const game::LobbyMsg* lobby_msg, const std::uint64_t sender_id, const game::LobbyModule module)
	{
		const auto ip_str{ utils::string::adr_to_string(from) };
		const auto type_name{ game::LobbyTypes_GetMsgTypeName(lobby_msg->type) };
		const auto lobby_message{ "Received lobby message [%i] <%s> from (%llu) %s" };

		if (!log_messages)
		{
			return logger::print_log(lobby_message, module, type_name, sender_id, ip_str.data());
		}

		PRINT_LOG(lobby_message, module, type_name, sender_id, ip_str.data());
	}

	bool handle_message(const game::netadr_t* from, game::LobbyMsg* lobby_msg, const std::uint64_t sender_id, const game::LobbyModule module)
	{
		log_lobby_messages(from, lobby_msg, sender_id, module);

		if (lobby_msg->msg.cursize == lobby_msg->msg.readcount)
		{
			PRINT_MESSAGE("Received invalid message from (%llu) %s", sender_id, utils::string::adr_to_string(from).data());
			return true;
		}

		if (const auto message_func{ messages.find(module) }; message_func != messages.end())
		{
			return message_func->second(*from, *lobby_msg, sender_id);
		}

		return false;
	}
	
	bool __fastcall callback_handle_packet_internal(game::netadr_t* from, game::msg_t* msg, std::uint64_t sender_id, game::LobbyModule module)
	{
		if (game::LobbyMsg lobby_msg{}; game::LobbyMsgRW_PrepReadMsg(&lobby_msg, msg))
		{
			if (handle_message(from, &lobby_msg, sender_id, module))
			{
				return true;
			}
		}

		return false;
	}

	size_t handle_packet_internal()
	{
		static auto stub = utils::hook::assemble([](auto& a)
		{
			const auto return_original = a.newLabel();

			a.mov(ecx, ptr(rbp, 0x77));

			a.pushad64();

			a.mov(r9, ecx); // lobby_module
			a.mov(r8, rsi); // sender_id
			a.mov(rdx, rdi); // msg
			a.mov(rcx, r14); // netadr

			a.call_aligned(lobby_msg::callback_handle_packet_internal);

			a.test(al, al);
			a.jz(return_original);

			a.popad64();
			a.jmp(game::base_address + 0x38F7CE1);

			a.bind(return_original);
			a.popad64();
			a.jmp(game::base_address + 0x38F7B4B);
		});

		return reinterpret_cast<size_t>(stub);
	}

	void initialize()
	{
		exception::hbp::register_exception(game::base_address + 0x38F7B48, [](const LPEXCEPTION_POINTERS ex)
		{
			ex->ContextRecord->Rip = lobby_msg::handle_packet_internal();
			return EXCEPTION_CONTINUE_EXECUTION;
		});
	}
}