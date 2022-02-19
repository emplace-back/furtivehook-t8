#include "dependencies/std_include.hpp"
#include "events.hpp"

namespace events
{
	void initialize()
	{
		instant_message::initialize();
		lobby_msg::initialize();
	}
}