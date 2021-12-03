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
		bool send(const game::netadr_t & target, const std::string & data, const game::netsrc_t & sock = game::NS_SERVER);
		bool register_remote_addr(const game::XSESSION_INFO * info, netadr_t * addr);
		bool register_remote_addr(const InfoResponseLobby& lobby, netadr_t* addr);
	}
	
	void initialize();
	void bold_game_message(const char * msg, ...);
	XSESSION_INFO get_session_info(const InfoResponseLobby& lobby);

	extern std::unordered_map<std::string, bool> handlers; 
	extern dvar_t* com_smoothframes_original;

	const static auto base_address = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr)) + 0x1000;
	
	const static auto Sys_GetTLS = reinterpret_cast<TLSData*(*)()>(base_address + 0x3C558E0);
	const static auto dwInstantSendMessage = reinterpret_cast<void(*)(ControllerIndex_t, const std::uint64_t*, unsigned int, char, const void*, unsigned int)>(base_address + 0x2ED1150);
	const static auto LobbyMsgRW_PrepWriteMsg = reinterpret_cast<bool(*)(LobbyMsg*, char*, int, MsgType, char)>(base_address + 0x38F7220);
	const static auto Live_IsUserSignedInToDemonware = reinterpret_cast<bool(*)(const ControllerIndex_t)>(base_address + 0x38030F0);
	const static auto MSG_InitReadOnly = reinterpret_cast<void(*)(msg_t*, const char*, int)>(base_address + 0x2DDE230);
	const static auto MSG_BeginReading = reinterpret_cast<void(*)(msg_t*)>(base_address + 0x2DDDCB0);
	const static auto MSG_ReadByte = reinterpret_cast<std::uint8_t(*)(msg_t*)>(base_address + 0x2DDE700);
	const static auto MSG_ReadData = reinterpret_cast<void(*)(msg_t*, void*, unsigned int, unsigned int)>(base_address + 0x2DDE940);
	const static auto LobbyMsgRW_PrepReadData = reinterpret_cast<bool(*)(LobbyMsg*, char*, int)>(base_address + 0x38F71C0);
	const static auto LobbyMsgRW_PackageUInt = reinterpret_cast<bool(*)(LobbyMsg *, const char *, const unsigned int*)>(base_address + 0x38F6FE0);
	const static auto MSG_InfoResponse = reinterpret_cast<bool(*)(void*, LobbyMsg *)>(base_address + 0x38DC1A0);
	const static auto Com_Error = reinterpret_cast<void(*)(const char*, int, int, const char*)>(base_address + 0x288A510);
	const static auto NET_OutOfBandPrint = reinterpret_cast<bool(*)(netsrc_t, netadr_t, const char *)>(base_address + 0x2E055A0);
	const static auto dwCommonAddrToNetadr = reinterpret_cast<bool(*)(netadr_t* const, const void* const, const bdSecurityID*)>(base_address + 0x2ED2660);
	const static auto dwRegisterSecIDAndKey = reinterpret_cast<bool(*)(const bdSecurityID*, const bdSecurityKey*)>(base_address + 0x2ED4640);
	const static auto dwNetadrToCommonAddr = reinterpret_cast<bool(*)(netadr_t, void*, const unsigned int, bdSecurityID*)>(base_address + 0x2ED4050);
	const static auto Cbuf_AddText = reinterpret_cast<void(*)(LocalClientNum_t, const char*)>(base_address + 0x3CDE020);
	const static auto CG_BoldGameMessage = reinterpret_cast<void(*)(LocalClientNum_t, const char*)>(base_address + 0x1481510);
	const static auto LiveUser_IsXUIDLocalPlayer = reinterpret_cast<bool(*)(const std::uint64_t)>(game::base_address + 0x38C1FE0);
	const static auto MSG_Init = reinterpret_cast<void(*)(game::msg_t*, char*, int)>(game::base_address + 0x2DDE1A0);
	const static auto MSG_WriteByte = reinterpret_cast<void(*)(game::msg_t*, int)>(game::base_address + 0x2DE48C0);
	const static auto MSG_WriteData = reinterpret_cast<void(*)(game::msg_t*, const void *, int)>(game::base_address + 0x2DE48E0);
	const static auto onGlobalInstantMessage = reinterpret_cast<void(*)(const dwLobbyEventHandler*, std::uint64_t, const char*, void*, unsigned int)>(game::base_address + 0x2ED03E0);
	const static auto CL_ControllerIndex_GetUsername = reinterpret_cast<const char*(*)(ControllerIndex_t)>(game::base_address + 0x441BAE0);
	
	const static auto& window = *reinterpret_cast<HWND*>(base_address + 0xFC671B0);
	const static auto& swap_chain = reinterpret_cast<IDXGISwapChain**>(base_address + 0xBD71910);
}