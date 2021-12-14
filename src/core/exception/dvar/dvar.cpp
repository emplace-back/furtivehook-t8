#include "dependencies/std_include.hpp"
#include "dvar.hpp"

namespace exception::dvar
{
	namespace
	{
		std::unordered_map<int, callback>& get_callbacks()
		{
			static std::unordered_map<int, callback> callbacks{};
			return callbacks;
		}
	}

	void register_exception(const int index, const callback& callback)
	{
		get_callbacks()[index] = callback;
	}

	bool handle_exception(const LPEXCEPTION_POINTERS ex)
	{
		const auto& callbacks = get_callbacks();
		const auto handler = callbacks.find(ex->ContextRecord->Rcx);

		if (handler == callbacks.end())
		{
			return false;
		}

		handler->second(ex);
		return true;
	}

	void initialize()
	{
		utils::hook::set<std::uintptr_t>(game::base_address + 0x8B50458, dvar::main);
	}
}