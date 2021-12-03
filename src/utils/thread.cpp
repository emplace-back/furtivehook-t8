#include "dependencies/std_include.hpp"
#include "thread.hpp"

namespace utils::thread
{
	std::vector<DWORD> get_thread_ids()
	{
		auto* const handle{ CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0) };
		
		if (handle == INVALID_HANDLE_VALUE)
		{
			return {};
		}

		THREADENTRY32 entry{};
		entry.dwSize = sizeof(entry);

		std::vector<DWORD> ids{};
		
		if (Thread32First(handle, &entry))
		{
			do
			{
				if (entry.th32ThreadID != GetCurrentThreadId() && entry.th32OwnerProcessID == GetCurrentProcessId())
				{
					ids.emplace_back(entry.th32ThreadID);
				}
			} while (Thread32Next(handle, &entry));
		}

		CloseHandle(handle);
		return ids;
	}

	void for_each_thread(const std::function<void(HANDLE)>& callback)
	{
		const auto ids{ get_thread_ids() };

		for (const auto& id : ids)
		{
			auto* const thread{ OpenThread(THREAD_ALL_ACCESS, FALSE, id) };
			
			if (thread != nullptr)
				callback(thread);

			CloseHandle(thread);
		}
	}
}