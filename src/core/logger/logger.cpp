#include "dependencies/std_include.hpp"
#include "logger.hpp"

namespace logger
{
	void print_log(const char* msg, ...)
	{
		char buffer[2048] = { 0 };

		va_list ap;
		va_start(ap, msg);

		vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

		va_end(ap);

		const static auto filename{ utils::generate_log_filename("furtivehook\\logs\\") };
		utils::io::write_file(filename, buffer, true);
	}
}