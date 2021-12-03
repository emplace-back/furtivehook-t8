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
	
	void register_exception(const exception_index& address, const callback & callback);
	void initialize();

	extern std::unordered_map<exception_index, callback> exceptions;
}