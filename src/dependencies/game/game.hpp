#pragma once
#include "structs.hpp"
#include "offsets.hpp"
#include "core/logger/logger.hpp"
#include "utils/utils.hpp"
#include "utils/spoof_call/spoof_call.hpp"

namespace game
{
	namespace oob
	{
		bool send(const netadr_t & target, const std::string & data, const netsrc_t & sock = NS_SERVER);
		bool register_remote_addr(XSESSION_INFO* info, netadr_t * addr);
		bool register_remote_addr(const InfoResponseLobby& lobby, netadr_t* addr);
		void send_lobby_msg(const game::netadr_t & to, const msg_t * lobby_msg, const LobbyModule module);
	}
	
	void initialize();
	void bold_game_message(const char * msg, ...);
	XSESSION_INFO get_session_info(const InfoResponseLobby& lobby);
	void send_instant_message(const std::vector<std::uint64_t>& recipients, std::uint8_t type, const void * message, const std::uint32_t message_size);

	extern LobbySession* session;
	extern dvar_t* com_smoothframes_original;

	const static auto base_address = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr)) + 0x1000;
	
	const static auto Sys_GetTLS = reinterpret_cast<TLSData*(*)()>(base_address + 0x3C558E0);
	const static auto dwInstantSendMessage = reinterpret_cast<void(*)(ControllerIndex_t, const std::uint64_t*, unsigned int, char, const void*, unsigned int)>(base_address + 0x2ED1150);
	const static auto LobbyMsgRW_PrepWriteMsg = reinterpret_cast<bool(*)(msg_t*, char*, int, MsgType, char)>(base_address + 0x38F7220);
	const static auto MSG_InitReadOnly = reinterpret_cast<void(*)(msg_t*, std::uint8_t*, int)>(base_address + 0x2DDE230);
	const static auto MSG_BeginReading = reinterpret_cast<void(*)(msg_t*)>(base_address + 0x2DDDCB0);
	const static auto MSG_ReadByte = reinterpret_cast<std::uint8_t(*)(msg_t*)>(base_address + 0x2DDE700);
	const static auto MSG_ReadData = reinterpret_cast<void(*)(msg_t*, void*, unsigned int, unsigned int)>(base_address + 0x2DDE940);
	const static auto LobbyMsgRW_PrepReadData = reinterpret_cast<bool(*)(msg_t*, char*, int)>(base_address + 0x38F71C0);
	const static auto LobbyMsgRW_PackageUInt = reinterpret_cast<bool(*)(msg_t *, const char *, const unsigned int*)>(base_address + 0x38F6FE0);
	const static auto MSG_InfoResponse = reinterpret_cast<bool(*)(void*, msg_t *)>(base_address + 0x38DC1A0);
	const static auto Com_Error = reinterpret_cast<void(*)(const char*, int, int, const char*)>(base_address + 0x288A510);
	const static auto NET_OutOfBandPrint = reinterpret_cast<bool(*)(netsrc_t, netadr_t, const char *)>(base_address + 0x2E055A0);
	const static auto dwCommonAddrToNetadr = reinterpret_cast<bool(*)(netadr_t* const, const void* const, const bdSecurityID*)>(base_address + 0x2ED2660);
	const static auto dwRegisterSecIDAndKey = reinterpret_cast<bool(*)(const bdSecurityID*, const bdSecurityKey*)>(base_address + 0x2ED4640);
	const static auto dwNetadrToCommonAddr = reinterpret_cast<bool(*)(netadr_t, void*, const unsigned int, bdSecurityID* const)>(base_address + 0x2ED4050);
	const static auto Cbuf_AddText = reinterpret_cast<void(*)(LocalClientNum_t, const char*)>(base_address + 0x3CDE020);
	const static auto CG_BoldGameMessage = reinterpret_cast<void(*)(LocalClientNum_t, const char*)>(base_address + 0x1481510);
	const static auto LiveUser_IsXUIDLocalPlayer = reinterpret_cast<bool(*)(const std::uint64_t)>(base_address + 0x38C1FE0);
	const static auto MSG_Init = reinterpret_cast<void(*)(msg_t*, char*, int)>(base_address + 0x2DDE1A0);
	const static auto MSG_WriteByte = reinterpret_cast<void(*)(msg_t*, int)>(base_address + 0x2DE48C0);
	const static auto MSG_WriteData = reinterpret_cast<void(*)(msg_t*, const void *, int)>(base_address + 0x2DE48E0);
	const static auto MSG_WriteString = reinterpret_cast<void(*)(msg_t*, const char*)>(base_address + 0x2DF8D90);
	const static auto MSG_WriteShort = reinterpret_cast<void(*)(msg_t*, int)>(base_address + 0x2DF8AA0);
	const static auto LobbySession_GetClientByClientNum = reinterpret_cast<SessionClient*(*)(LobbySession *, ClientNum_t)>(base_address + 0x3904C60);
	const static auto LobbyActiveList_GetCommonAddr = reinterpret_cast<bdCommonAddr **(*)(bdCommonAddr **, ActiveClient *)>(base_address + 0x38FE170);
	const static auto LobbyMsgRW_PrepReadMsg = reinterpret_cast<bool(*)(msg_t *, msg_t*)>(base_address + 0x38F71F0);
	const static auto LobbyTypes_GetMsgTypeName = reinterpret_cast<const char*(*)(const MsgType)>(base_address + 0x38E9780);
	const static auto serialize = reinterpret_cast<void(*)(const game::bdCommonAddr *, void *)>(base_address + 0x44905A0);
	const static auto LobbyMsgRW_PackageInt = reinterpret_cast<bool(*)(msg_t *, const char*, const int*)>(base_address + 0x38F6D50);
	const static auto LobbyMsgRW_PackageGlob = reinterpret_cast<bool(*)(msg_t *, const char*, void*, int)>(base_address + 0x38F6BF0);
	const static auto LobbyMsgRW_PackageXuid = reinterpret_cast<bool(*)(msg_t *, const char*, const std::uint64_t*)>(base_address + 0x38F7120);
	const static auto LobbyMsgRW_PackageUChar = reinterpret_cast<bool(*)(msg_t *, const char*, const std::uint8_t*)>(base_address + 0x38F6EB0);
	const static auto NET_OutOfBandData = reinterpret_cast<bool(*)(netsrc_t, netadr_t, const char *, int)>(base_address + 0x2E05490);
	const static auto LobbyJoinSource_JoinProbed = reinterpret_cast<bool(*)(const int, const ControllerIndex_t, bool, std::uint64_t, const char *, LobbyType, const bdSecurityID *, const bdSecurityKey *, const SerializedAdr *, JoinType, std::uint64_t)>(base_address + 0x38E0330);
	const static auto Com_SessionMode_IsMode = reinterpret_cast<bool(*)(eModes)>(base_address + 0x289E280);
	const static auto Com_IsInGame = reinterpret_cast<bool(*)()>(base_address + 0x288EEB0);
	const static auto LobbyMsgRW_PackageArrayStart = reinterpret_cast<bool(*)(msg_t *, const char*)>(base_address + 0x38F68E0);
	const static auto LobbyMsgRW_PackageElement = reinterpret_cast<bool(*)(msg_t *, bool)>(base_address + 0x38F6AC0);
	const static auto SendClientReliableData = reinterpret_cast<bool(*)(const ControllerIndex_t, int, Msg_ClientReliableData *)>(base_address + 0x38D07E0);
	const static auto Com_IsRunningUILevel = reinterpret_cast<bool(*)()>(base_address + 0x288EEF0);
	const static auto Live_IsDemonwareFetchingDone = reinterpret_cast<bool(*)(const ControllerIndex_t)>(base_address + 0x3803010);
	const static auto lobbymsg_prints = reinterpret_cast<game::dvar_t**>(game::base_address + 0xE1770E8);
	const static auto onGlobalInstantMessage = reinterpret_cast<void(*)(dwLobbyEventHandler*, std::uint64_t, const char*, const void*, unsigned int)>(base_address + 0x2ED03E0);
	const static auto& s_infoProbe = *reinterpret_cast<InfoProbe*>(base_address + 0xE175570);
	const static auto& window = *reinterpret_cast<HWND*>(base_address + 0xFC671B0);
	const static auto& swap_chain = reinterpret_cast<IDXGISwapChain**>(base_address + 0xBD71910);

	inline std::uintptr_t get_offset(const std::uintptr_t address)
	{
		constexpr auto IDA_BASE_ADDRESS{ 0x7FF7338A1000u }; 
		const auto offset{ address - IDA_BASE_ADDRESS };
		
		return base_address + offset;
	}
	
	template<typename T> T& get_offset(const std::uintptr_t address)
	{
		return *reinterpret_cast<T*>(get_offset(address));
	}
	
	inline LobbySession* get_host_session(const LobbyType lobbyType) {
		return reinterpret_cast<LobbySession*>(base_address + 0xEB736B0 + 0x31A10 * lobbyType);
	}
	
	inline LobbySession* get_client_session(const LobbyType lobbyType) {
		return reinterpret_cast<LobbySession*>(base_address + 0xEB3D060 + sizeof(LobbySession) * lobbyType);
	}

	inline userData_t* get_user_data() {
		const static auto LiveUser_GetUserDataForController = reinterpret_cast<userData_t*(*)(const ControllerIndex_t)>(base_address + 0x38C1C00);
		return LiveUser_GetUserDataForController(0);
	}
}