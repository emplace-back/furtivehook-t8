#include "dependencies/std_include.hpp"
#include "menu.hpp"

namespace menu
{
	bool initialized = false, open = false;

	void set_style_color()
	{
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = { 1.0f, 1.0f, 1.0f, 1.0f };
		colors[ImGuiCol_TextDisabled] = { 0.784f, 0.784f, 0.784f, 0.784f };
		colors[ImGuiCol_WindowBg] = { 0.06f, 0.06f, 0.06f, 1.00f };
		colors[ImGuiCol_PopupBg] = { 0.08f, 0.08f, 0.08f, 0.94f };
		colors[ImGuiCol_Border] = { 0.00f, 0.00f, 0.00f, 0.71f };
		colors[ImGuiCol_BorderShadow] = { 0.06f, 0.06f, 0.06f, 0.01f };
		colors[ImGuiCol_FrameBg] = { 0.10f, 0.10f, 0.10f, 0.71f };
		colors[ImGuiCol_FrameBgHovered] = { 0.19f, 0.19f, 0.19f, 0.40f };
		colors[ImGuiCol_FrameBgActive] = { 0.20f, 0.20f, 0.20f, 0.67f };
		colors[ImGuiCol_TitleBg] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_MenuBarBg] = { 0.10f, 0.10f, 0.10f, 0.66f };
		colors[ImGuiCol_ScrollbarBg] = { 0.02f, 0.02f, 0.02f, 0.00f };
		colors[ImGuiCol_ScrollbarGrab] = { 0.31f, 0.31f, 0.31f, 1.00f };
		colors[ImGuiCol_ScrollbarGrabHovered] = { 0.17f, 0.17f, 0.17f, 1.00f };
		colors[ImGuiCol_ScrollbarGrabActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_CheckMark] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_SliderGrab] = { 0.29f, 0.29f, 0.29f, 1.00f };
		colors[ImGuiCol_SliderGrabActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_Button] = { 0.10f, 0.10f, 0.10f, 1.00f };
		colors[ImGuiCol_ButtonHovered] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_ButtonActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_Header] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_HeaderActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_Separator] = { 0.10f, 0.10f, 0.10f, 0.90f };
		colors[ImGuiCol_SeparatorHovered] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_SeparatorActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_ResizeGrip] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_ResizeGripHovered] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_ResizeGripActive] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
		colors[ImGuiCol_PlotLinesHovered] = { 1.00f, 1.00f, 1.00f, 1.00f };
		colors[ImGuiCol_PlotHistogram] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_PlotHistogramHovered] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_TextSelectedBg] = { 0.0f, 0.392f, 0.784f, 1.0f };
		colors[ImGuiCol_ModalWindowDarkening] = { 0.80f, 0.80f, 0.80f, 0.35f };
	}

	void set_style()
	{
		auto& style = ImGui::GetStyle();
		ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->AddFontDefault();

		set_style_color();

		style.WindowPadding = { 8.0f, 2.0f };
		style.FramePadding = { 4.0f , 4.0f };
		style.ItemSpacing = { 10.0f, 4.0f };
		style.ItemInnerSpacing = { 4.0f, 4.0f };
		style.TouchExtraPadding = {};
		style.IndentSpacing = 21.0f;
		style.ScrollbarSize = 13.0f;
		style.GrabMinSize = 10.0f;
		style.WindowBorderSize = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupBorderSize = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.TabBorderSize = 1.0f;

		style.WindowRounding = 7.0f;
		style.ChildRounding = 7.0f;
		style.FrameRounding = 7.0f;
		style.PopupRounding = 7.0f;
		style.ScrollbarRounding = 7.0f;
		style.GrabRounding = 7.0f;
		style.TabRounding = 7.0f;

		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.SelectableTextAlign = {};

		style.DisplaySafeAreaPadding = { 4.0f, 4.0f };

		style.ColumnsMinSpacing = 6.0f;
		style.CurveTessellationTol = 1.25f;
		style.AntiAliasedLines = true;
	}

	void toggle()
	{
		if (initialized)
		{
			open = !open;
			utils::hook::set<bool>(game::base_address + 0xFC60795, !open);
		}
	}

	bool is_open()
	{
		return initialized && open;
	}

	bool begin_section(const std::string& text)
	{
		ImGui::TextUnformatted(text.data());
		return true;
	}

	void draw()
	{
		if (is_open())
		{
			ImGui::SetNextWindowSize({ 480, 480 }, ImGuiCond_Once);

			if (!ImGui::Begin(window_title, nullptr, window_flags))
			{
				ImGui::End();
				return;
			}

			if (ImGui::BeginTabBar("tabs"))
			{
				const auto width = ImGui::GetContentRegionAvail().x;
				const auto spacing = ImGui::GetStyle().ItemInnerSpacing.x; 
				
				ImGui::Separator();

				if (ImGui::BeginTabItem("Misc"))
				{
					ImGui::Checkbox("Log out-of-band packets", &events::connectionless_packet::log_packets); 
					ImGui::Checkbox("Log instant messages", &events::instant_message::dispatch::log_messages);
					
					if (ImGui::CollapsingHeader("Exploits", ImGuiTreeNodeFlags_Leaf))
					{
						static auto steam_id_input = ""s;

						ImGui::SetNextItemWidth(width * 0.85f);
						ImGui::InputTextWithHint("##target_steam_id", "ID", &steam_id_input);

						const auto target_steam_id{ utils::atoll(steam_id_input) };

						if (ImGui::MenuItem("Send crash", nullptr, nullptr, target_steam_id && !steam_id_input.empty()))
						{
							exploit::instant_message::send_info_response_overflow(target_steam_id);
						}

						if (ImGui::MenuItem("Send popup", nullptr, nullptr, target_steam_id && !steam_id_input.empty()))
						{
							exploit::instant_message::send_popup(target_steam_id);
						}

						if (ImGui::MenuItem("Send IQ", nullptr, nullptr, target_steam_id && !steam_id_input.empty()))
						{
							std::vector<std::uint64_t> recipients;
							recipients.emplace_back(target_steam_id);
							
							events::instant_message::send_info_request(recipients);
						}
					}
					
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Extras"))
				{
					if (ImGui::CollapsingHeader("Tools", ImGuiTreeNodeFlags_Leaf))
					{
						if (begin_section("Execute command"))
						{
							static auto command_input = ""s;

							ImGui::SetNextItemWidth(width * 0.85f);
							ImGui::InputTextWithHint("##command_input", "Command", &command_input);

							ImGui::SameLine();

							if (ImGui::Button("Execute##execute_command", { 64.0f, 0.0f }))
							{
								game::Cbuf_AddText(0, command_input.data());
							}
						}
					}
					
					ImGui::EndTabItem();
				}

				friends::draw_friends_list(width, spacing); 
			}
		}
	}

	void initialize(IDXGISwapChain* swap_chain)
	{
		if (!initialized)
		{
			initialized = true;

			rendering::dx::on_frame(swap_chain);
			input::on_key(VK_F1, toggle, true);

			friends::refresh_friends();
		}
	}
}