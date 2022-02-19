#pragma once
#include "dependencies/std_include.hpp"

namespace exception::hwbp
{
	using callback = std::function<size_t()>; 
	
	bool handle_exception(const LPEXCEPTION_POINTERS ex);
	void initialize();
}