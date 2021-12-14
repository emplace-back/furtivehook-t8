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
	}

	void register_exception(const std::uintptr_t address, const callback& callback)
	{
		get_callbacks()[address] = callback;
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
		utils::thread::for_each_thread([](const HANDLE thread)
		{
			CONTEXT context{};
			context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

			if (!GetThreadContext(thread, &context))
			{
				return;
			}

			context.Dr0 = game::base_address + 0x2ED0FE2;
			context.Dr1 = game::base_address + 0x2856539;
			context.Dr2 = game::base_address + 0x38F7B48;
			context.Dr7 = (1 << 0) | (1 << 2) | (1 << 4);

			SetThreadContext(thread, &context);
		});
	}
}