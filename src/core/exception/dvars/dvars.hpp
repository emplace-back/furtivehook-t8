#pragma once
#include "dependencies/std_include.hpp"

namespace exception::dvars
{
	enum hook_dvar
	{
		com_frame = 1,
		scr_update_frame = 2,
		count = 2,
	}; 
	
	using callback = std::function<void(CONTEXT&)>;
	
	bool handle_exception(const LPEXCEPTION_POINTERS ex);
	void initialize();
}