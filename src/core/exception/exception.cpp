#include "dependencies/std_include.hpp"
#include "exception.hpp"

namespace exception
{
	thread_local struct
	{
		DWORD code = 0;
		PVOID address = nullptr;
	} exception_data;
	
	void page_guard_address(const std::uintptr_t address)
	{
		DWORD old_protect{ PAGE_EXECUTE | PAGE_GUARD };
		VirtualProtect(reinterpret_cast<void*>(address), sizeof std::uint8_t, old_protect, &old_protect);
	}
	
	void reset_state()
	{
		auto error = "Termination due to a stack overflow."s;
		if (exception_data.code != EXCEPTION_STACK_OVERFLOW)
		{
			error = utils::string::va("Exception: 0x%08X at offset 0x%llX", exception_data.code, DWORD64(exception_data.address) - game::base_address);
		}

		PRINT_LOG("%s", error.data()); 
	}
	
	size_t get_reset_state_stub()
	{
		const static auto stub = utils::hook::assemble([](auto& a)
		{
			a.sub(rsp, 0x10);
			a.or_(rsp, 0x8);
			a.jmp(reset_state);
		});

		return reinterpret_cast<size_t>(stub);
	}
	
	LONG __stdcall exception_filter(const LPEXCEPTION_POINTERS ex)
	{
		const auto code = ex->ExceptionRecord->ExceptionCode;
		const auto addr = ex->ExceptionRecord->ExceptionAddress;
		
		if (code == STATUS_INTEGER_OVERFLOW || code == STATUS_FLOAT_OVERFLOW)
		{
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		
		if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		{
			const auto index = static_cast<dvar::exception_index>(ex->ContextRecord->Rcx);

			if (const auto exception_func = dvar::exceptions.find(index); exception_func != dvar::exceptions.end())
			{
				exception_func->second(ex);
				return EXCEPTION_CONTINUE_EXECUTION;
			}
		}
		else if (ex->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
		{
			if (const auto exception_func = hbp::exceptions.find(ex->ContextRecord->Rip); exception_func != hbp::exceptions.end())
			{
				return exception_func->second(ex);
			}
		}
		
		utils::exception::minidump::write_minidump(ex);

		exception_data = { code, addr };
		ex->ContextRecord->Rip = get_reset_state_stub();

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	LPTOP_LEVEL_EXCEPTION_FILTER __stdcall set_unhandled_exception_filter(LPTOP_LEVEL_EXCEPTION_FILTER)
	{
		return &exception_filter;
	}

	void initialize()
	{
		SetUnhandledExceptionFilter(exception_filter);
		utils::hook::jump(&SetUnhandledExceptionFilter, &set_unhandled_exception_filter, true);

		exception::hbp::initialize();
		exception::dvar::initialize();
	}
}