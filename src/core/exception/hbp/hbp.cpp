#include "dependencies/std_include.hpp"
#include "hbp.hpp"

namespace exception::hbp
{
	std::unordered_map<std::uintptr_t, callback> exceptions;

	void register_exception(const std::uintptr_t address, const callback& callback)
	{
		if (exceptions.find(address) == exceptions.end())
		{
			exceptions[address] = callback;
		}
	}
	
	void initialize()
	{
		utils::thread::for_each_thread([](const HANDLE thread)
		{
			CONTEXT context{};
			context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			if (!GetThreadContext(thread, &context))
			{
				return;
			}

			context.Dr0 = game::base_address + 0x2ED0F84;
			context.Dr1 = game::base_address + 0x2856562;
			context.Dr2 = game::base_address + 0x38F7B48;
			context.Dr7 = (1 << 0) | (1 << 2) | (1 << 4);

			SetThreadContext(thread, &context);
		});
	}
}