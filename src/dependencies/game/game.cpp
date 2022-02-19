#include "dependencies/std_include.hpp"

namespace game
{
	LobbySession* session = nullptr;
	
	namespace oob
	{
		bool send(const netadr_t& target, const std::string& data, const netsrc_t& sock)
		{
			return NET_OutOfBandData(sock, target, data.data(), data.size());
		}

		bool register_remote_addr(const XSESSION_INFO* info, netadr_t* addr)
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

		void send_lobby_msg(const game::netadr_t& to, const msg_t* msg, const LobbyModule module)
		{
			auto data{ events::lobby_msg::build_lobby_msg(module) };
			data.append(reinterpret_cast<char*>(msg->data), msg->cursize);
			
			send(to, data);
		}
	}
	
	void initialize()
	{
		rendering::initialize();
		exception::initialize();
		events::initialize();
		security::initialize();
		misc::initialize();
		friends::initialize();

		PRINT_LOG("Initialized!");
	}
	
	XSESSION_INFO get_session_info(const InfoResponseLobby& lobby)
	{
		XSESSION_INFO sess_info{};
		sess_info.sessionID = lobby.secId;
		sess_info.keyExchangeKey = lobby.secKey;
		sess_info.hostAddress = lobby.serializedAdr.xnaddr;

		return sess_info;
	}

	void send_instant_message(const std::vector<std::uint64_t>& recipients, std::uint8_t type, const void* message, const std::uint32_t message_size)
	{
		return dwInstantSendMessage(0, recipients.data(), recipients.size(), type, message, message_size);
	}
}