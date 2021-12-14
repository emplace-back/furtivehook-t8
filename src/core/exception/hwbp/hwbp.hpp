#pragma once
#include "dependencies/std_include.hpp"

namespace exception::hwbp
{
	using callback = std::function<size_t()>; 
	
	void register_exception(const std::uintptr_t address, const callback& callback);
	bool handle_exception(const LPEXCEPTION_POINTERS ex);
	void initialize();
	
	extern std::unordered_map<std::uintptr_t, callback> exceptions;
}