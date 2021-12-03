#pragma once
#include "dependencies/std_include.hpp"

namespace exception 
{
	void page_guard_address(const std::uintptr_t address);
	void initialize();
}