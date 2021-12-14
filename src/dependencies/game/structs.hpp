#pragma once
#include "dependencies/std_include.hpp"

#define DOT_PRODUCT(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define ANGLE2SHORT(a) ((int)((a)*(65536/360.0f))&65535)
#define SHORT2ANGLE(a) ((float)((a)*(360.0f/65536)))
#define M_PI 3.14159265358979323846
#define M_PI_DOUBLE ((float)M_PI*2.0f)
#define DEG2RAD(a) ((a)*((float)M_PI/180.0f))
#define RAD2DEG(a) ((a)*(180.0f/(float)M_PI))
#define GET_SIGN(a) ((a) ? ((*(int*)&(a) >> 31) | 1) : 0)
#define VECTOR_COPY(b, a) ((b)[0]=(a)[0],(b)[1]=(a)[1],(b)[2]=(a)[2])

class Vec2 {
public:
	float x, y;

	Vec2() :x(0), y(0) {}
	Vec2(const float x, const float y) : x(x), y(y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}
	Vec2(const float fl[2]) : x(fl[0]), y(fl[1]) {}

	operator float*() { return &x; }
	operator const float*() const { return &x; }

	bool operator==(const Vec2& v) const { return x == v.x && y == v.y; };
	bool operator!=(const Vec2& v) const { return !(*this == v); };

	Vec2 operator-() const { return Vec2(-x, -y); }
	Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
	Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
	Vec2 operator*(const float fl) const { return Vec2(x * fl, y * fl); }
	Vec2 operator/(const float fl) const { return Vec2(x / fl, y / fl); }

	void operator+=(const Vec2& v) { x += v.x; y += v.y; }
	void operator-=(const Vec2& v) { x -= v.x; y -= v.y; }
	void operator*=(const Vec2& v) { x *= v.x; y *= v.y; }
	void operator*=(const float fl) { x *= fl; y *= fl; }
	void operator/=(const Vec2& v) { x /= v.x; x /= v.y; }

	void set(const float x, const float y) {
		this->x = x;
		this->y = y;
	}

	void rotate(const double deg) {
		const double theta = deg / 180.0 * M_PI;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	void clear() {
		this->x = 0.0f; this->y = 0.0f;
	}

	bool empty() const {
		return this->x == 0.0f && this->y == 0.0f;
	}

	Vec2& normalize() {
		if (length() == 0) return *this;
		*this *= (1.0f / length());
		return *this;
	}

	float distance(const Vec2& v) const {
		Vec2 d(x - v.x, y - v.y);
		return d.length();
	}

	float length() const {
		return std::sqrtf(x * x + y * y);
	}
};

class Vec3
{
public:
	float x, y, z;

	Vec3() :x(0), y(0), z(0) {}
	Vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}
	Vec3(const float fl[3]) : x(fl[0]), y(fl[1]), z(fl[3]) {}

	operator float* () { return &x; }
	operator const float* () const { return &x; }

	bool operator==(const Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
	bool operator!=(const Vec3& v) const { return !(*this == v); }

	Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }
	Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator*(const float fl) const { return Vec3(x * fl, y * fl, z * fl); }
	Vec3 operator/(const float fl) const { return Vec3(x / fl, y / fl, z / fl); }

	void operator += (const Vec3& add) { x += add.x; y += add.y; z += add.z; }
	void operator -= (const Vec3& sub) { x -= sub.x; y -= sub.y; z -= sub.z; }
	void operator *= (const float mul) { x *= mul; y *= mul; z *= mul; }
	void operator /= (const float mul) { x /= mul; y /= mul; z /= mul; }

	void set(const float value) {
		x = value; y = value; z = value;
	}

	void clear() {
		set(0.0f);
	}

	float length() const {
		return std::sqrtf(x * x + y * y + z * z);
	}

	bool empty() const {
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	float dot() const {
		return (x * x + y * y + z * z);
	}

	Vec3 cross_product(const Vec3& other) const {
		return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
	}

	Vec3& normalized() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}

	float distance(Vec3 v) const {
		Vec3 d(x - v.x, y - v.y, z - v.z);
		return d.length();
	}
};

using LocalClientNum_t = std::uint32_t;
using scr_string_t = std::uint32_t;
using ControllerIndex_t = std::uint32_t;
using ClientNum_t = std::uint32_t;

namespace game
{
	enum JoinType
	{
		JOIN_TYPE_NORMAL = 0x0,
		JOIN_TYPE_PLAYLIST = 0x1,
		JOIN_TYPE_FRIEND = 0x2,
		JOIN_TYPE_INVITE = 0x3,
		JOIN_TYPE_PARTY = 0x4,
		JOIN_TYPE_GROUPS = 0x5,
		JOIN_TYPE_COUNT = 0x6,
	};
	
	enum LobbyType
	{
		LOBBY_TYPE_INVALID = 0xFFFFFFFF,
		LOBBY_TYPE_PRIVATE = 0x0,
		LOBBY_TYPE_GAME = 0x1,
		LOBBY_TYPE_TRANSITION = 0x2,
		LOBBY_TYPE_COUNT = 0x3,
	}; 
	
	enum errorCode
	{
		ERROR_NONE = 0x0,
		ERROR_FATAL = 0x1,
		ERROR_DROP = 0x2,
		ERROR_FROM_STARTUP = 0x4,
		ERROR_SERVERDISCONNECT = 0x8,
		ERROR_DISCONNECT = 0x10,
		ERROR_SCRIPT = 0x20,
		ERROR_SCRIPT_DROP = 0x40,
		ERROR_LOCALIZATION = 0x80,
		ERROR_UI = 0x100,
		ERROR_LUA = 0x200,
		ERROR_SOFTRESTART = 0x400,
		ERROR_SOFTRESTART_KEEPDW = 0x800,
		ERROR_SOFTRESTART_SILENT = 0x1000,
	}; 
	
	enum IMType
	{
		JOIN_REQUEST = 0x0,
		JOIN_REPLY = 0x1,
		INVITE = 0x2,
		UPDATE_INVITE_INFO = 0x3,
		NUM_INSTANT_MESSAGE_TYPES = 0x4,
	}; 

	enum PackageType
	{
		PACKAGE_TYPE_NONE = 0x0,
		PACKAGE_TYPE_WRITE = 0x1,
		PACKAGE_TYPE_READ = 0x2,
	}; 

	enum MsgType
	{
		MESSAGE_TYPE_NONE = -1,
		MESSAGE_TYPE_INFO_REQUEST = 0,
		MESSAGE_TYPE_INFO_RESPONSE = 1,
		MESSAGE_TYPE_LOBBY_STATE_PRIVATE = 2,
		MESSAGE_TYPE_LOBBY_STATE_GAME = 3,
		MESSAGE_TYPE_LOBBY_STATE_TRANSITION = 4,
		MESSAGE_TYPE_LOBBY_HOST_HEARTBEAT = 5,
		MESSAGE_TYPE_LOBBY_HOST_DISCONNECT = 6,
		MESSAGE_TYPE_LOBBY_HOST_DISCONNECT_CLIENT = 7,
		MESSAGE_TYPE_LOBBY_HOST_LEAVE_WITH_PARTY = 8,
		MESSAGE_TYPE_LOBBY_CLIENT_HEARTBEAT = 10,
		MESSAGE_TYPE_LOBBY_CLIENT_DISCONNECT = 11,
		MESSAGE_TYPE_LOBBY_CLIENT_RELIABLE_DATA = 12,
		MESSAGE_TYPE_LOBBY_CLIENT_CONTENT = 13,
		MESSAGE_TYPE_LOBBY_MODIFIED_STATS = 14,
		MESSAGE_TYPE_JOIN_LOBBY = 15,
		MESSAGE_TYPE_JOIN_RESPONSE = 16,
		MESSAGE_TYPE_JOIN_AGREEMENT_REQUEST = 17,
		MESSAGE_TYPE_JOIN_AGREEMENT_RESPONSE = 18,
		MESSAGE_TYPE_JOIN_COMPLETE = 19,
		MESSAGE_TYPE_JOIN_MEMBER_INFO = 20,
		MESSAGE_TYPE_PEER_TO_PEER_CONNECTIVITY_TEST = 22,
		MESSAGE_TYPE_PEER_TO_PEER_INFO = 23,
		MESSAGE_TYPE_LOBBY_MIGRATE_ANNOUNCE_HOST = 25,
		MESSAGE_TYPE_LOBBY_MIGRATE_START = 26,
		MESSAGE_TYPE_INGAME_MIGRATE_TO = 27,
		MESSAGE_TYPE_INGAME_MIGRATE_NEW_HOST = 28,
		MESSAGE_TYPE_VOICE_PACKET = 29,
		MESSAGE_TYPE_COUNT = 33,
	};

	enum netadrtype_t
	{
		NA_BOT = 0x0,
		NA_BAD = 0x1,
		NA_LOOPBACK = 0x2,
		NA_RAWIP = 0x3,
		NA_IP = 0x4,
	};
	
	enum netsrc_t
	{
		NS_NULL = 0xFFFFFFFF,
		NS_CLIENT1 = 0x0,
		NS_CLIENT2 = 0x1,
		NS_CLIENT3 = 0x2,
		NS_CLIENT4 = 0x3,
		NS_SERVER = 0x4,
		NS_MAXCLIENTS = 0x4,
		NS_PACKET = 0x5,
	}; 

	enum LobbyNetworkMode
	{
		LOBBY_NETWORKMODE_INVALID = 0xFFFFFFFF,
		LOBBY_NETWORKMODE_UNKNOWN = 0x0,
		LOBBY_NETWORKMODE_LAN = 0x1,
		LOBBY_NETWORKMODE_LIVE = 0x2,
		LOBBY_NETWORKMODE_COUNT = 0x3,
		LOBBY_NETWORKMODE_LOCAL = 0x1,
	};

	enum LobbyMainMode
	{
		LOBBY_MAINMODE_INVALID = 0xFFFFFFFF,
		LOBBY_MAINMODE_CP = 0x0,
		LOBBY_MAINMODE_MP = 0x1,
		LOBBY_MAINMODE_ZM = 0x2,
		LOBBY_MAINMODE_COUNT = 0x3,
	};

	enum eModes
	{
		MODE_ZOMBIES = 0x0,
		MODE_MULTIPLAYER = 0x1,
		MODE_CAMPAIGN = 0x2,
		MODE_COUNT = 0x3,
		MODE_INVALID = 0x3,
		MODE_FIRST = 0x0,
	};

	enum dvarType_t
	{
		DVAR_TYPE_INVALID = 0x0,
		DVAR_TYPE_BOOL = 0x1,
		DVAR_TYPE_FLOAT = 0x2,
		DVAR_TYPE_FLOAT_2 = 0x3,
		DVAR_TYPE_FLOAT_3 = 0x4,
		DVAR_TYPE_FLOAT_4 = 0x5,
		DVAR_TYPE_INT = 0x6,
		DVAR_TYPE_ENUM = 0x7,
		DVAR_TYPE_STRING = 0x8,
		DVAR_TYPE_COLOR = 0x9,
		DVAR_TYPE_INT64 = 0xA,
		DVAR_TYPE_UINT64 = 0xB,
		DVAR_TYPE_LINEAR_COLOR_RGB = 0xC,
		DVAR_TYPE_COLOR_XYZ = 0xD,
		DVAR_TYPE_COLOR_LAB = 0xE,
		DVAR_TYPE_SESSIONMODE_BASE_DVAR = 0xF,
		DVAR_TYPE_COUNT = 0x10,
	};

	enum LobbyModule
	{
		LOBBY_MODULE_INVALID = 0xFFFFFFFF,
		LOBBY_MODULE_HOST = 0x0,
		LOBBY_MODULE_CLIENT = 0x1,
		LOBBY_MODULE_PEER_TO_PEER = 0x3,
		LOBBY_MODULE_COUNT = 0x4,
	};

	enum LobbyMode
	{
		LOBBY_MODE_INVALID = 0xFFFFFFFF,
		LOBBY_MODE_PUBLIC = 0x0,
		LOBBY_MODE_CUSTOM = 0x1,
		LOBBY_MODE_THEATER = 0x2,
		LOBBY_MODE_ARENA = 0x3,
		LOBBY_MODE_FREERUN = 0x4,
		LOBBY_MODE_COUNT = 0x5,
	};

	enum SessionActive
	{
		SESSION_INACTIVE = 0x0,
		SESSION_KEEP_ALIVE = 0x1,
		SESSION_ACTIVE = 0x2,
	};

	enum LobbyDisconnectClient
	{
		LOBBY_DISCONNECT_CLIENT_INVALID = 0xFFFFFFFF,
		LOBBY_DISCONNECT_CLIENT_DROP = 0x0,
		LOBBY_DISCONNECT_CLIENT_KICK = 0x1,
		LOBBY_DISCONNECT_CLIENT_BADDLC = 0x2,
		LOBBY_DISCONNECT_CLIENT_KICK_PARTY = 0x3,
		LOBBY_DISCONNECT_CLIENT_HOSTRELOAD = 0x4,
		LOBBY_DISCONNECT_CLIENT_NOPARTYCHAT = 0x5,
	};
	
	struct msg_t
	{
		char *data;
		int maxsize;
		int cursize;
		int readcount;
		char pad[0x21];
		bool overflowed;
		bool readOnly;
		char pad2[0x8];
		MsgType type;
		PackageType packageType;
		char encodeFlags;
	};

	struct JoinSessionMessage
	{
		IMType mType;
		char pad[0x74];
	};

	struct netadr_t
	{
		union
		{
			std::uint8_t ip[4];
			std::uint32_t inaddr;
		};

		std::uint16_t port;
		netadrtype_t type;
		netsrc_t localNetID;
	};

#pragma pack(push, 1)
	struct XNADDR
	{
		char pad[0x3C];
		union
		{
			std::uint8_t ip[4];
			std::uint32_t inaddr;
		};

		std::uint16_t port;
		char pad2[0x1];
	};
#pragma pack(pop)
	
	struct bdSecurityID
	{
		std::uint64_t id;
	}; 

	struct bdSecurityKey
	{
		char ab[16];
	};

	struct XSESSION_INFO
	{
		bdSecurityID sessionID;
		XNADDR hostAddress;
		bdSecurityKey keyExchangeKey;
	};
	
	struct LobbyParams
	{
		LobbyNetworkMode networkMode;
		LobbyMainMode mainMode;
	};

	struct SerializedAdr
	{
		bool valid;
		XNADDR xnaddr;
	}; 
	
	struct InfoResponseLobby
	{
		bool isValid;
		std::uint64_t hostXuid;
		char hostName[32];
		bdSecurityID secId;
		bdSecurityKey secKey;
		SerializedAdr serializedAdr;
		LobbyParams lobbyParams;
	}; 
	
	struct Msg_InfoResponse
	{
		std::uint32_t nonce;
		int uiScreen;
		std::uint8_t natType;
		InfoResponseLobby lobby[3];
	};

	struct $7034703ED3857507327AE195CCA24A71
	{
		int stringCount;
		const char **strings;
	};

	struct $5AC0CC375C7A91894540A0C75867866C
	{
		int min;
		int max;
	};

	struct $CE635080F256838B60294DD7E0543359
	{
		int64_t min;
		int64_t max;
	};

	struct $28DE879AA85D5F4AA5DF9392CB197229
	{
		uint64_t min;
		uint64_t max;
	};

	struct $D2B45DCE3C73414E1DE8E10A2D52B392
	{
		float min;
		float max;
	};

	struct $97F25DA469865168FD08682F42882F2A
	{
		float min;
		float max;
	};

	union DvarLimits
	{
		$7034703ED3857507327AE195CCA24A71 enumeration;
		$5AC0CC375C7A91894540A0C75867866C integer;
		$CE635080F256838B60294DD7E0543359 integer64;
		$28DE879AA85D5F4AA5DF9392CB197229 unsignedInt64;
		$D2B45DCE3C73414E1DE8E10A2D52B392 value;
		$97F25DA469865168FD08682F42882F2A vector;
	};

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		int64_t integer64;
		uint64_t unsignedInt64;
		float value;
		Vec3 vector;
		const char *string;
		float color[4];
	};

	struct dvar_t
	{
		unsigned int name;
		const char *debugName;
		const char *description;
		unsigned int flags;
		dvarType_t type;
		bool modified;
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarLimits domain;
		dvar_t *hashNext;
	};

	struct va_info_t
	{
		char va_string[4][1024];
		int index;
	};

	struct TraceCheckCount
	{
		unsigned __int16 global;
		unsigned __int16 *partitions;
		unsigned __int16 *brushes;
	};

	struct TraceThreadInfo
	{
		TraceCheckCount checkcount;
		void *box_brush;
		void *box_model;
		void **geoms;
	};

	struct CmdArgs
	{
		int nesting;
		LocalClientNum_t localClientNum[8];
		ControllerIndex_t controllerIndex[8];
		int argshift[8];
		int argc[8];
		const char **argv[8];
		char textPool[8192];
		const char *argvPool[512];
		int usedTextPool[8];
		int totalUsedArgvPool;
		int totalUsedTextPool;
	};

	struct TLSData
	{
		va_info_t *vaInfo;
		jmp_buf *errorJmpBuf;
		TraceThreadInfo *traceInfo;
		CmdArgs *cmdArgs;
		void *errorData;
	};

	struct SessionInfo
	{
		bool inSession;
		netadr_t netAdr;
		time_t lastMessageSentToPeer;
	}; 
	
	struct FixedClientInfo
	{
		std::uint64_t xuid;
		char pad2[0xAC];
		char gamertag[32];
	};

	struct ActiveClient
	{
		char pad[0x2C0];
		FixedClientInfo fixedClientInfo;
		SessionInfo sessionInfo[2];
	};
	
	struct SessionClient
	{
		char pad[0x18];
		ActiveClient* activeClient;
	}; 
	
	struct LobbySession
	{
		LobbyModule module;
		LobbyType type;
		LobbyMode mode;
		char pad[0x34];
		SessionActive active;
		char pad2[0x121D4];
	};

	enum bdNATType
	{
		BD_NAT_UNKNOWN = 0x0,
		BD_NAT_OPEN = 0x1,
		BD_NAT_MODERATE = 0x2,
		BD_NAT_STRICT = 0x3,
		BD_NAT_COUNT = 0x4,
	};

	struct bdReferencable
	{
		int(**_vptr$bdReferencable)(void);
		volatile long m_refCount;
	};

	struct $6741FC495A8B2E2BAD20C8172EB1EAD0
	{
		unsigned char m_b1;
		unsigned char m_b2;
		unsigned char m_b3;
		unsigned char m_b4;
	};

	struct $1623BBAA00FD6ABF956B3E4A1698C8C9
	{
		unsigned short m_w1;
		unsigned short m_w2;
		unsigned short m_w3;
		unsigned short m_w4;
		unsigned short m_w5;
		unsigned short m_w6;
		unsigned short m_w7;
		unsigned short m_w8;
	};


	union $7FA1600C02EC957451F4B78DDDD8EBD1
	{
		$7FA1600C02EC957451F4B78DDDD8EBD1::$6741FC495A8B2E2BAD20C8172EB1EAD0 m_caddr;
		unsigned int m_iaddr;
		$1623BBAA00FD6ABF956B3E4A1698C8C9 m_caddr6;
		unsigned char m_iaddr6[16];
		unsigned char m_sockaddr_storage[128];
	};


	struct bdSockAddr
	{
		bdSockAddr::$7FA1600C02EC957451F4B78DDDD8EBD1 inUn;
		unsigned short m_family;
	};

	struct bdInetAddr
	{
		bdSockAddr m_addr;
	};

	struct bdAddr
	{
		bdInetAddr m_address;
		unsigned short m_port;
	};

	struct bdLocalAddr
	{
		bdAddr *m_data;
		unsigned int m_capacity;
		unsigned int m_size;
	};

	struct bdCommonAddr : bdReferencable
	{
		bdLocalAddr m_localAddrs;
		bdAddr m_publicAddr;
		bdNATType m_natType;
		unsigned int m_hash;
		bool m_isLoopback;
	};

	struct bdCommonAddrRef
	{
		bdCommonAddr *m_ptr;
	};

	struct Msg_ConnectionTest
	{
		int lobbyType;
		int clientIndex;
	};

	struct userData_t
	{
		char pad[0x4];
		char gamertag[32];
		char pad2[0x14];
		int signInState;
		int connectionState;
	};

	struct MsgHostMigrateInfo
	{
		uint8_t indexBits;
		int32_t lasthostTimeMS;
		uint64_t migrateNominees[18];
	}; 
	
	struct Msg_LobbyHostHeartbeat
	{
		int heartbeatNum;
		int lobbyType;
		MsgHostMigrateInfo migrateInfo;
	};

	struct Msg_LobbyMigrateStart
	{
		int lobbyType;
		uint64_t migrateTo;
	};

	struct Msg_PeerToPeerInfo
	{
		int lobbyType;
		int connectivityBits;
		uint64_t clientXuid;
	};

	struct Msg_ClientReliableData
	{
		uint32_t dataMask;
		LobbyType lobbyType;
		uint64_t xuidNewLeader;
		uint64_t disconnectClientXuid;
		int disconnectClient;
		int leaderActivity;
		uint64_t platformSessionID;
		int lobbyTypeMoveFrom;
		int moveCount;
		uint64_t moveXuids[18];
		int team;
		int mapVote;
		bool readyUp;
		int itemVote;
		int characterSelection;
		bool editComplete;
	};

	struct InfoProbe
	{
		bool active;
		unsigned int nonce;
	};

	struct dwLobbyEventHandler
	{
		std::uint8_t baseclass_0[8];
		ControllerIndex_t m_controllerIndex;
	};
}
