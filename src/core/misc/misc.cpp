#include "dependencies/std_include.hpp"
#include "misc.hpp"

namespace misc
{
	void initialize()
	{
		input::on_key(VK_F2, [] { game::Cbuf_AddText(0, "disconnect"); });
		input::on_key(VK_F3, [] { game::Cbuf_AddText(0, "quit"); });
		
		scheduler::loop([]()
		{
			const auto lobby_session{ game::get_client_session(game::LOBBY_TYPE_GAME) };
			const auto party_session{ game::get_client_session(game::LOBBY_TYPE_PRIVATE) };
			
			if (lobby_session->active || (game::session = party_session, !party_session->active))
				game::session = lobby_session;
		}, scheduler::pipeline::main);
	}
}