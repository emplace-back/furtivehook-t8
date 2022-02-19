#include "dependencies/std_include.hpp"
#include "connectionless_packet.hpp"

namespace events::connectionless_packet
{
	bool log_commands = true;

	namespace
	{
		void log_dispatch_connectionless_packet_commands(const game::netadr_t* from)
		{
			const auto ip_str{ utils::string::adr_to_string(from) };
			const auto command{ command::args.join() };
			const auto message{ "Received OOB '%s' from %s" };

			if (!log_commands)
			{
				return logger::print_log(message, command.data(), ip_str.data());
			}

			PRINT_LOG(message, command.data(), ip_str.data());
		}

		std::unordered_map<std::string, callback>& get_callbacks()
		{
			static std::unordered_map<std::string, callback> callbacks{};
			return callbacks;
		}

		bool handle_command(const game::netadr_t* from, game::msg_t* msg)
		{
			if (const command::args_ args{};
				args.size() > 0)
			{
				const auto cmd_string = utils::string::to_lower(args[0]);
				const auto& callbacks = get_callbacks();
				const auto handler = callbacks.find(cmd_string);

				if (handler == callbacks.end())
				{
					log_dispatch_connectionless_packet_commands(from);
					return false;
				}

				const auto msg_backup = *msg;
				const auto cb = handler->second(args, *from, *msg);

				if (msg->readcount != msg_backup.readcount)
					*msg = msg_backup;

				return cb;
			}

			return false;
		}
	}

	size_t handle_command_stub()
	{
		const static auto stub = utils::hook::assemble([](auto& a)
		{
			const auto return_original = a.newLabel();

			a.movups(xmm0, xmmword_ptr(r15));

			a.pushad64();
			a.mov(rdx, r12); // msg
			a.mov(rcx, r15); // netadr
			a.call_aligned(connectionless_packet::handle_command);
			a.test(al, al);
			a.jz(return_original);
			a.popad64();

			a.jmp(game::base_address + 0x2856B47);

			a.bind(return_original);
			a.popad64();
			a.jmp(game::base_address + 0x285653D);
		});

		return reinterpret_cast<size_t>(stub);
	}

	void on_command(const std::string& command, const callback& callback)
	{
		get_callbacks()[utils::string::to_lower(command)] = callback;
	}
}