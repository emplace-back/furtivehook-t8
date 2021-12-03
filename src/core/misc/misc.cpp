#include "dependencies/std_include.hpp"
#include "misc.hpp"

namespace misc
{
	void initialize()
	{
		input::on_key(VK_F2, [] { game::Cbuf_AddText(0, "disconnect"); });
		input::on_key(VK_F3, [] { game::Cbuf_AddText(0, "quit"); });

		scheduler::once(scheduler::start_thread, scheduler::pipeline::main);
	}
}