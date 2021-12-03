#include "dependencies/std_include.hpp"

namespace game
{
	std::unordered_map<std::string, bool> handlers; 
	dvar_t* com_smoothframes_original = nullptr;
	
	namespace oob
	{
		bool send(const game::netadr_t& target, const std::string& data, const game::netsrc_t& sock)
		{
			return NET_OutOfBandPrint(sock, target, data.data());
		}

		bool register_remote_addr(const game::XSESSION_INFO* info, netadr_t* addr)
		{
			dwRegisterSecIDAndKey(&info->sessionID, &info->keyExchangeKey);

			if (!dwCommonAddrToNetadr(addr, &info->hostAddress, &info->sessionID))
			{
				return false;
			}

			return true;
		}

		bool register_remote_addr(const InfoResponseLobby& lobby, netadr_t* addr)
		{
			const auto session_info{ get_session_info(lobby) };
			return register_remote_addr(&session_info, addr);
		}
	}
	
	void initialize()
	{
		handlers["mstart"]; 
		handlers["connectResponseMigration"];
		handlers["echo"];
		handlers["print"];
		
		com_smoothframes_original = *reinterpret_cast<dvar_t**>(base_address + 0x8B50458);
		
		rendering::initialize();
		scheduler::initialize();
		security::initialize();
		security::iat::initialize();
		exception::initialize();
		events::instant_message::initialize();
		events::connectionless_packet::initialize();
		misc::initialize();

		PRINT_LOG("Initialized!");
	}

	void bold_game_message(const char* msg, ...)
	{
		char buffer[2048];

		va_list ap;
		va_start(ap, msg);

		vsnprintf_s(buffer, sizeof(buffer), _TRUNCATE, msg, ap);

		va_end(ap);

		CG_BoldGameMessage(0, buffer);
	}
	
	XSESSION_INFO get_session_info(const InfoResponseLobby& lobby)
	{
		game::XSESSION_INFO sess_info{};
		sess_info.sessionID = lobby.secId;
		sess_info.keyExchangeKey = lobby.secKey;
		sess_info.hostAddress = lobby.serializedAdr.xnaddr;

		return sess_info;
	}
}