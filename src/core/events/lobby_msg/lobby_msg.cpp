#include "dependencies/std_include.hpp"
#include "lobby_msg.hpp"

namespace events::lobby_msg
{
	bool log_messages = false;
	
	namespace
	{
		bool handle_host_disconnect_client(const game::netadr_t& from, game::msg_t& msg)
		{
			PRINT_MESSAGE("Disconnect prevented from %s", utils::string::adr_to_string(&from).data());
			return true;
		}

		bool handle_host_msg(const game::netadr_t& from, game::msg_t& msg)
		{
			if (msg.type == game::MESSAGE_TYPE_LOBBY_HOST_DISCONNECT_CLIENT)
			{
				return handle_host_disconnect_client(from, msg);
			}

			return false;
		}
		
		void log_lobby_messages(const game::netadr_t* from, const game::msg_t* msg, const game::LobbyModule module)
		{
			const auto ip_str{ utils::string::adr_to_string(from) };
			const auto type_name{ game::LobbyTypes_GetMsgTypeName(msg->type) };
			const auto lobby_message{ "Received lobby message [%i] <%s> from %s" };

			if (!log_messages)
			{
				return logger::print_log(lobby_message, module, type_name, ip_str.data());
			}

			PRINT_LOG(lobby_message, module, type_name, ip_str.data());
		}
		
		std::unordered_map<game::LobbyModule, callback>& get_callbacks()
		{
			static std::unordered_map<game::LobbyModule, callback> callbacks{};
			return callbacks;
		}

		bool __fastcall handle_packet(game::netadr_t* from, game::msg_t* msg, game::LobbyModule module)
		{
			const auto& callbacks = get_callbacks();
			const auto handler = callbacks.find(module);

			if (handler == callbacks.end())
			{
				log_lobby_messages(from, msg,  module);
				return false;
			}

			return handler->second(*from, *msg);
		}

		size_t handle_packet_stub()
		{
			const static auto stub = utils::hook::assemble([](auto& a)
			{
				const auto return_original = a.newLabel();

				a.mov(ecx, ptr(rbp, 0x77));

				a.pushad64();
				a.mov(r8, ecx); // lobby_module
				a.lea(rdx, ptr(rbp, -0x39)); // msg
				a.mov(rcx, r14); // netadr
				a.call_aligned(lobby_msg::handle_packet);
				a.test(al, al);
				a.jz(return_original);
				a.popad64();
				
				a.jmp(game::base_address + 0x38F7CAD);

				a.bind(return_original);
				a.popad64();
				a.jmp(game::base_address + 0x38F7B4B);
			});

			return reinterpret_cast<size_t>(stub);
		}
	}

	std::string build_lobby_msg(const game::LobbyModule module)
	{
		auto data{ "LM"s };
		data.push_back(0);
		constexpr auto header{ 0x928ui16 };
		data.append(reinterpret_cast<const char*>(&header), sizeof header);
		data.push_back(static_cast<std::uint8_t>(module));
		data.push_back(-1);

		return data;
	}

	void on_message(const game::LobbyModule& module, const callback& callback)
	{
		get_callbacks()[module] = callback;
	}

	void initialize()
	{
		exception::hwbp::register_exception(game::base_address + 0x38F7B48, lobby_msg::handle_packet_stub);

		events::lobby_msg::on_message(game::LOBBY_MODULE_CLIENT, &handle_host_msg);
	}
}