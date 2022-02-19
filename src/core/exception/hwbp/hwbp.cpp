#include "dependencies/std_include.hpp"
#include "hwbp.hpp"

namespace exception::hwbp
{
	namespace
	{
		std::unordered_map<std::uintptr_t, callback>& get_callbacks()
		{
			static std::unordered_map<std::uintptr_t, callback> callbacks{};
			return callbacks;
		}

		int get_register_index(const CONTEXT& ctx)
		{
			for (size_t i = 0; i < sizeof(std::uint32_t); ++i)
			{
				const auto a = i * sizeof(std::uint16_t);
				const auto has_index = (ctx.Dr7 & (1 << a)) == 0;

				if (has_index)
				{
					return i;
				}
			}

			return -1;
		}

		void register_hook(const std::uintptr_t address, const callback& callback)
		{
			utils::thread::set_registers_for_each_thread([=](auto& ctx)
			{
				const auto index{ get_register_index(ctx) };

				if (index < 0 || index >= 4)
				{
					return;
				}

				auto* debug_register = reinterpret_cast<size_t*>(&ctx.Dr0);
				debug_register[index] = address;

				ctx.Dr7 |= (1 << index * 2);
				ctx.Dr7 |= (0b00 << (16 + (index * 2) - 1)); // set condition type (16-17, 21-20, 24-25, 28-29)
				ctx.Dr7 &= ~(1 << 18 + (index * 2)); // set size (18-19, 22-23, 26-27, 30-31)
			});

			get_callbacks()[address] = callback;
		}
	}

	bool handle_exception(const LPEXCEPTION_POINTERS ex)
	{
		const auto& callbacks = get_callbacks();
		const auto handler = callbacks.find(ex->ContextRecord->Rip);

		if (handler == callbacks.end())
		{
			return false;
		}

		ex->ContextRecord->Rip = handler->second(); 
		return true;
	}

	void initialize()
	{
		hwbp::register_hook(game::base_address + 0x2856539, events::connectionless_packet::handle_command_stub);
		hwbp::register_hook(game::base_address + 0x2ED0FE2, events::instant_message::dispatch_message_stub);
		hwbp::register_hook(game::base_address + 0x38F7B48, events::lobby_msg::handle_packet_stub);
	}
}