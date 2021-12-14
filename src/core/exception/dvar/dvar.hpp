#pragma once
#include "dependencies/std_include.hpp"

namespace exception::dvar
{
	enum exception_index
	{
		main = 1, 
		count = 2,
	};
	
	using callback = std::function<void(const LPEXCEPTION_POINTERS)>;
	
	void register_exception(const int index, const callback & callback);
	bool handle_exception(const LPEXCEPTION_POINTERS ex);
	void initialize();

	extern std::unordered_map<exception_index, callback> exceptions;
}