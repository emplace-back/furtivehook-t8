#include "dependencies/std_include.hpp"
#include "connectionless_packet.hpp"

namespace events::connectionless_packet
{
	std::unordered_map<std::string, callback> commands;
	bool log_packets = true;

	void on_command(const std::string& command, const callback& callback)
	{
		if (commands.find(command) == commands.end())
		{
			commands[utils::string::to_lower(command)] = callback;
		}
	}

	bool handle_command(const std::string& command, game::netadr_t* from, game::msg_t& msg)
	{
		const auto cmd_string{ utils::string::to_lower(command) };

		if (const auto command_func{ commands.find(cmd_string) }; command_func != commands.end())
		{
			const command::args_ args{};

			if (args.size() > 0)
			{
				const auto msg_backup{ msg };
				const auto cb{ command_func->second(args, *from, msg) };

				if(msg.readcount != msg_backup.readcount)
					msg = msg_backup;

				return cb;
			}
		}

		return false;
	}

	void log_dispatch_connectionless_packet_commands(game::netadr_t* from)
	{
		const auto message{ "Received OOB '%s' from %s" }; 
		
		if (!log_packets)
		{
			return logger::print_log(message, command::args.join().data(), utils::string::adr_to_string(from).data());
		}

		PRINT_LOG(message, command::args.join().data(), utils::string::adr_to_string(from).data());
	}

	bool __fastcall callback_cl_dispatch_connectionless_packet(const char* command, game::netadr_t* from, game::msg_t* msg)
	{
		connectionless_packet::log_dispatch_connectionless_packet_commands(from); 
		
		if (connectionless_packet::handle_command(command, from, *msg))
		{
			return true;
		}

		return false;
	}
	
	size_t cl_dispatch_connectionless_packet()
	{
		const static auto stub = utils::hook::assemble([](auto& a)
		{
			const auto return_original = a.newLabel(); 
				
			a.pushad64();

			a.mov(r8, r12); // msg
			a.mov(rdx, r15); // netadr
			a.mov(rcx, rdi); // command

			a.call_aligned(connectionless_packet::callback_cl_dispatch_connectionless_packet);
			
			a.test(al, al);
			a.jz(return_original); 
			
			a.popad64();
			a.jmp(game::base_address + 0x2856BFA);
			
			a.bind(return_original);
			a.popad64(); 
			a.jmp(game::base_address + 0x2856565);
		});

		return reinterpret_cast<size_t>(stub);
	}

	void initialize()
	{
		exception::hbp::register_exception(game::base_address + 0x2856562, [](const LPEXCEPTION_POINTERS ex)
		{
			ex->ContextRecord->R9 = ex->ContextRecord->R12;
			ex->ContextRecord->Rip = connectionless_packet::cl_dispatch_connectionless_packet();
			return EXCEPTION_CONTINUE_EXECUTION;
		});
	}
}