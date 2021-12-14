#include "dependencies/std_include.hpp"
#include "events.hpp"

namespace events
{
	void initialize()
	{
		events::connectionless_packet::initialize();
		events::instant_message::initialize();
		events::lobby_msg::initialize();
	}
}