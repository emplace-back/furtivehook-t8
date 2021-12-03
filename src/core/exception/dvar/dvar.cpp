#include "dependencies/std_include.hpp"
#include "dvar.hpp"

namespace exception::dvar
{
	std::unordered_map<exception_index, callback> exceptions;

	void register_exception(const exception_index& index, const callback& callback)
	{
		if (exceptions.find(index) == exceptions.end())
		{
			exceptions[index] = callback;
		}
	}

	void initialize()
	{
		utils::hook::set<std::uintptr_t>(game::base_address + 0x8B50458, dvar::main);
	}
}