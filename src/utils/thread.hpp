#pragma once
#include "dependencies/std_include.hpp"

namespace utils::thread
{
	void for_each_thread(const std::function<void(HANDLE)>& callback);
}
