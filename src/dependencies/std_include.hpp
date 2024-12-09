#pragma once

#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <intrin.h>
#include <algorithm>
#include <WinInet.h>
#include <time.h>
#include <dxgi.h>
#include <random>
#include <numeric>
#include <future>
#include <mutex>
#include <regex>
#include <cmath>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <array>
#include <codecvt>
#include <psapi.h>
#include <Shlwapi.h>
#include <dbghelp.h>
#include <filesystem>
#include <shellapi.h>
#include <cctype>
#include <csetjmp>
#include <sstream>
#include <TlHelp32.h>
#include <utils/json/nlohmann/json.hpp>
#include <utils/asmjit/core/jitruntime.h>
#include <utils/asmjit/x86/x86assembler.h>
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace std::chrono_literals;
using namespace std::literals;
using json = nlohmann::json;

#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_internal.h"
#include "../thirdparty/imgui/imgui_impl_dx11.h"
#include "../thirdparty/imgui/imgui_impl_win32.h"

#include "game/game.hpp"
#include "math/math.hpp"

#include "utils/utils.hpp"
#include "utils/io.hpp"
#include "utils/hook.hpp"
#include "utils/memory.hpp"
#include "utils/string.hpp"
#include "utils/concurrency.hpp"
#include "utils/nt.hpp"
#include "utils/thread.hpp"
#include "utils/toast.hpp"
#include "utils/exception/minidump.hpp"
#include "utils/spoof_call/spoof_call.hpp"

#include "core/misc/misc.hpp"
#include "core/input/input.hpp"
#include "core/rendering/dx.hpp"
#include "core/rendering/rendering.hpp"
#include "core/menu/menu.hpp"
#include "core/friends/friends.hpp"
#include "core/scheduler/scheduler.hpp"
#include "core/loader/loader.hpp"
#include "core/security/iat/iat.hpp"
#include "core/security/security.hpp"
#include "core/command/command.hpp"
#include "core/exploit/exploit.hpp"
#include "core/rendering/rendering.hpp"
#include "core/exception/hwbp/hwbp.hpp"
#include "core/exception/dvars/dvars.hpp"
#include "core/exception/exception.hpp"
#include "core/logger/logger.hpp"
#include "core/events/instant_message/instant_message.hpp"
#include "core/events/connectionless_packet/connectionless_packet.hpp"
#include "core/events/lobby_msg/lobby_msg.hpp"
#include "core/events/events.hpp"