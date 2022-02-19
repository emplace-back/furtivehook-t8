#include "dependencies/std_include.hpp"
#include "toast.hpp"

namespace utils::toast
{
	std::vector<toast_data> toasts{}; 
	
	namespace
	{
		constexpr static auto window_title = "toast_window";
		constexpr static auto window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoFocusOnAppearing;
		constexpr static auto message_padding = 10.0f;
		
		float get_fade_percentage(const toast_data& toast)
		{
			const auto delta = std::chrono::high_resolution_clock::now() - toast.last_time;
			const auto fade_in_out_time = 0.8s;

			if (delta > toast.interval + fade_in_out_time * sizeof std::uint16_t)
			{
				return -1;
			} 
			else if (delta < fade_in_out_time)
			{
				return static_cast<float>(delta / fade_in_out_time);
			}
			else if (delta > toast.interval + fade_in_out_time)
			{
				const auto scale = delta - fade_in_out_time - toast.interval;
				return 1.0f - static_cast<float>(scale / fade_in_out_time);
			}

			return 1.0f;
		}
	}

	void add_toast(const std::string& title, const std::string& message, const std::chrono::milliseconds& interval)
	{
		add_toast({ title, message, interval, std::chrono::high_resolution_clock::now() });
	}
	
	void add_toast(const toast_data& toast)
	{
		toasts.emplace_back(toast);
	}

	void draw_toast()
	{
		const auto size = ImGui::GetIO().DisplaySize;
		auto height = 0.f;

		std::vector<size_t> indices{};

		for (size_t i = 0; i < toasts.size(); ++i)
		{
			indices.emplace_back(i);
		}

		std::sort(indices.begin(), indices.end(), [](const auto& a, const auto& b) { return toasts[a].title < toasts[b].title; });

		for (const auto& toast_num : indices)
		{
			const auto toast = toasts[toast_num]; 
			
			const auto opacity = get_fade_percentage(toast);

			if (opacity < 0)
			{
				toasts.erase(toasts.begin() + toast_num);
				continue;
			}

			ImGui::SetNextWindowBgAlpha(opacity);
			ImGui::SetNextWindowPos({ size.x - 20.0f, size.y - 1350.0f - height }, ImGuiCond_Always, { 1.0f, 1.0f });

			if (!ImGui::Begin(window_title + "##"s + std::to_string(toast_num), nullptr, window_flags))
			{
				ImGui::End();
				return;
			}

			ImGui::PushTextWrapPos(size.x * 0.35f);

			ImGui::PushFont(menu::glacial_indifference_bold);
			ImGui::TextUnformatted(toast.title);
			ImGui::PopFont();

			ImGui::PushFont(menu::glacial_indifference);
			ImGui::TextUnformatted(toast.message);
			ImGui::PopFont();

			ImGui::PopTextWrapPos();

			height -= ImGui::GetWindowHeight() + message_padding;
		}
	}
}