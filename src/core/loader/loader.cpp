#include "loader.hpp"

namespace loader
{
	namespace
	{
		std::unordered_map<std::string, callback>& get_callbacks()
		{
			static std::unordered_map<std::string, callback> callbacks{};
			return callbacks;
		}
		
		void* get_import_func(const std::string& library, const std::string& function_name)
		{
			const auto& callbacks = get_callbacks();
			const auto handler = callbacks.find(library);

			if (handler != callbacks.end() && handler->first == library)
			{
				return handler->second(function_name);
			}

			return nullptr;
		}

		void load_imports(const utils::nt::library& target, const utils::nt::library& source)
		{
			const auto import_directory{ &source.get_optional_header()->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT] };
			auto descriptor{ PIMAGE_IMPORT_DESCRIPTOR(target.get_ptr() + import_directory->VirtualAddress) };

			while (descriptor->Name)
			{
				const std::string name{ LPSTR(target.get_ptr() + descriptor->Name) };
				auto name_table_entry{ reinterpret_cast<uintptr_t*>(target.get_ptr() + descriptor->OriginalFirstThunk) };
				auto address_table_entry{ reinterpret_cast<uintptr_t*>(target.get_ptr() + descriptor->FirstThunk) };

				if (!descriptor->OriginalFirstThunk)
				{
					name_table_entry = reinterpret_cast<uintptr_t*>(target.get_ptr() + descriptor->FirstThunk);
				}

				while (*name_table_entry)
				{
					auto function_name{ ""s };

					if (IMAGE_SNAP_BY_ORDINAL(*name_table_entry))
					{
						function_name = "#" + std::to_string(IMAGE_ORDINAL(*name_table_entry));
					}
					else
					{
						const auto import{ PIMAGE_IMPORT_BY_NAME(target.get_ptr() + *name_table_entry) };
						function_name = import->Name;
					}

					if (const auto import_func{ get_import_func(name, function_name) }; import_func)
					{
						utils::hook::set<std::uintptr_t>(address_table_entry, reinterpret_cast<uintptr_t>(import_func));
					}

					name_table_entry++;
					address_table_entry++;
				}

				descriptor++;
			}
		}
	}

	void on_import(const std::string& library, const callback& callback)
	{
		get_callbacks()[library] = callback;
	}

	void load_library(const std::string& filename)
	{
		const auto target{ utils::nt::library::load(filename) };

		if (!target)
		{
			throw std::runtime_error{ "Failed to map binary!" };
		}

		return load_imports(target, target);
	}
}