#include "dependencies/std_include.hpp"
#include "menu.hpp"

namespace menu
{
	bool initialized = false, open = false;
	ImFont* glacial_indifference_bold;
	ImFont* glacial_indifference;
	
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
		auto& style{ ImGui::GetStyle() };
		ImGui::GetIO().FontDefault = ImGui::GetIO().Fonts->AddFontDefault();

		glacial_indifference_bold = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(ImGui::GetIO().Fonts->GetSecondaryFont().data(), 18.0f);
		glacial_indifference = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(ImGui::GetIO().Fonts->GetPrimaryFont().data(), 18.0f);

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
	
	void draw_player_list(const float width, const float spacing)
	{
		const auto session{ game::session };

		if (session == nullptr)
		{
			return;
		}
		
		if (ImGui::BeginTabItem("Player List"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
			ImGui::BeginColumns("Players", 3, ImGuiColumnsFlags_NoResize);

			ImGui::SetColumnWidth(-1, 28.0f);
			ImGui::TextUnformatted("#");
			ImGui::NextColumn();
			ImGui::TextUnformatted("Player");
			ImGui::NextColumn();
			ImGui::SetColumnOffset(-1, width - ImGui::CalcTextSize("Priority").x);
			ImGui::TextUnformatted("Priority");
			ImGui::NextColumn();

			ImGui::Separator();

			std::array<size_t, 18> indices{};

			for (size_t i = 0; i < 18; ++i)
			{
				indices[i] = i;
			}

			for (const auto& client_num : indices)
			{
				const auto target_client{ game::LobbySession_GetClientByClientNum(session, client_num) };

				if (target_client && target_client->activeClient)
				{
					ImGui::AlignTextToFramePadding();
					ImGui::TextUnformatted(std::to_string(client_num).data());

					ImGui::NextColumn();

					const auto player_xuid{ target_client->activeClient->fixedClientInfo.xuid };
					const auto player_name{ target_client->activeClient->fixedClientInfo.gamertag };

					ImGui::PushStyleColor(ImGuiCol_Text, game::LiveUser_IsXUIDLocalPlayer(player_xuid)
						? ImColor(0, 255, 127, 250).Value : ImColor(200, 200, 200, 250).Value);

					ImGui::AlignTextToFramePadding();

					const auto selected{ ImGui::Selectable((player_name + "##"s + std::to_string(client_num)).data()) };

					ImGui::PopStyleColor();

					const auto netadr{ target_client->activeClient->sessionInfo[session->type].netAdr };

					if (netadr.type == game::NA_BOT)
					{
						ImGui::SameLine(0, spacing);

						ImGui::TextColored(ImColor(200, 200, 200, 250).Value, "[BOT]");
					}
					
					const auto popup{ "player_popup##" + std::to_string(client_num) };
					static auto friend_player_name{ ""s };

					if (selected)
					{
						friend_player_name = player_name;
						ImGui::OpenPopup(popup.data());
					}
					
					if (ImGui::BeginPopup(popup.data(), ImGuiWindowFlags_NoMove))
					{
						if (ImGui::BeginMenu(player_name + "##"s + std::to_string(client_num) + "player_menu"))
						{
							ImGui::MenuItem(player_name + "##"s + std::to_string(client_num) + "player_menu_item", nullptr, false, false);

							if (ImGui::IsItemClicked())
							{
								ImGui::LogToClipboardUnformatted(player_name);
							}

							if (ImGui::IsItemHovered())
							{
								ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
							}

							if (ImGui::BeginMenu("Add to friends list##" + std::to_string(client_num)))
							{
								ImGui::InputTextWithHint("##" + std::to_string(client_num), "Name", &friend_player_name);

								if (ImGui::Button("Add friend"))
								{
									friends::friends.emplace_back(friends::friends_t{ player_xuid, friend_player_name });
									friends::write_to_friends();
								}

								ImGui::EndMenu();
							}

							ImGui::EndMenu();
						}

						ImGui::Separator();

						if (ImGui::MenuItem(std::to_string(player_xuid)))
						{
							ImGui::LogToClipboardUnformatted(std::to_string(player_xuid));
						}
						
						const auto is_netadr_valid{ netadr.inaddr && netadr.type != game::NA_BAD };
						const auto ip_string{ is_netadr_valid ? utils::string::adr_to_string(&netadr) : "Invalid IP Data" };
						
						if (ImGui::MenuItem(ip_string, nullptr, nullptr, is_netadr_valid))
						{
							ImGui::LogToClipboardUnformatted(ip_string);
						}

						ImGui::Separator();

						if (ImGui::MenuItem("Crash player", nullptr, nullptr, is_netadr_valid))
						{
							exploit::send_crash(netadr);
						}

						if (ImGui::BeginMenu("Exploits##" + std::to_string(client_num)))
						{
							if (ImGui::MenuItem("Remove from party"))
							{
								exploit::lobby_msg::send_disconnect_client(player_xuid);
							}
							
							if (ImGui::MenuItem("Show migration screen", nullptr, nullptr, is_netadr_valid))
							{
								exploit::send_mstart_packet(netadr);
							}
							
							if (ImGui::MenuItem("Kick from lobby", nullptr, nullptr, is_netadr_valid))
							{
								exploit::send_connect_response_migration_packet(netadr);
							}

							if (ImGui::BeginMenu("Send OOB##" + std::to_string(client_num), is_netadr_valid))
							{
								static auto string_input{ ""s };

								ImGui::InputTextWithHint("##" + std::to_string(client_num), "OOB", &string_input);

								if (ImGui::MenuItem("Send OOB"))
								{
									game::oob::send(netadr, string_input);
								}

								ImGui::EndMenu();
							}

							ImGui::EndMenu();
						}

						ImGui::EndPopup();
					}

					ImGui::NextColumn();

					ImGui::TextUnformatted("");

					ImGui::NextColumn();

					if (ImGui::GetColumnIndex() == 0)
					{
						ImGui::Separator();
					}
				}
			}

			ImGui::PopStyleVar();
			ImGui::EndColumns();
			ImGui::EndTabItem();
		}
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
				const auto width{ ImGui::GetContentRegionAvail().x };
				const auto spacing{ ImGui::GetStyle().ItemInnerSpacing.x };
				
				ImGui::Separator();

				if (ImGui::BeginTabItem("Misc"))
				{
					ImGui::Checkbox("Log out-of-band packets", &events::connectionless_packet::log_commands); 
					ImGui::Checkbox("Log instant messages", &events::instant_message::log_messages);
					ImGui::Checkbox("Log lobby messages", &events::lobby_msg::log_messages);
					
					if (ImGui::CollapsingHeader("Exploits", ImGuiTreeNodeFlags_Leaf))
					{
						static auto target_id_input{ ""s };

						ImGui::SetNextItemWidth(width * 0.85f);
						ImGui::InputTextWithHint("##target_id", "ID", &target_id_input);

						const auto target_id{ utils::atoll(target_id_input) };

						if (ImGui::MenuItem("Send crash", nullptr, nullptr, target_id && !target_id_input.empty()))
						{
							exploit::instant_message::send_info_response_overflow({ target_id });
						}

						if (ImGui::MenuItem("Send popup", nullptr, nullptr, target_id && !target_id_input.empty()))
						{
							exploit::instant_message::send_popup({ target_id });
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
							static auto command_input{ ""s };

							ImGui::SetNextItemWidth(width * 0.85f);
							ImGui::InputTextWithHint("##command_input", "Command", &command_input);

							ImGui::SameLine();

							if (ImGui::Button("Execute##execute_command", { 64.0f, 0.0f }))
							{
								game::Cbuf_AddText(0, command_input.data());
							}
						}
						
						if (begin_section("Search ID via gamertag"))
						{
							static auto gamertag_input{ ""s }; 
							
							ImGui::SetNextItemWidth(width * 0.85f);
							ImGui::InputTextWithHint("##gamertag_input", "Gamertag", &gamertag_input); 

							ImGui::SameLine();

							if (ImGui::Button("Execute##execute_lookup", { 64.0f, 0.0f }))
							{
								game::LiveGroups_SearchPlayerByGamertag(0, gamertag_input.data());
							}

							if (const auto search_results = game::get_player_search_results(0);
								search_results != nullptr 
								&& search_results->m_userID
								&& search_results->m_userName == gamertag_input)
							{
								const auto player_xuid = std::to_string(search_results->m_userID);

								ImGui::MenuItem(player_xuid, nullptr, false, false);

								if (ImGui::IsItemClicked())
								{
									ImGui::LogToClipboardUnformatted(player_xuid);
								}

								if (ImGui::IsItemHovered())
								{
									ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
								}
							}
						}

						if (begin_section("Send toast popup"))
						{
							static auto title_input{ ""s };

							ImGui::SetNextItemWidth(width * 0.85f);
							ImGui::InputTextWithHint("##title_input", "Title", &title_input);

							static auto message_input{ ""s };

							ImGui::SetNextItemWidth(width * 0.85f);
							ImGui::InputTextWithHint("##message_input", "Message", &message_input);

							if (ImGui::MenuItem("Send popup", nullptr, nullptr, !title_input.empty() && !message_input.empty()))
							{
								utils::toast::add_toast(title_input, message_input);
							}
						}
					}
					
					ImGui::EndTabItem();
				}

				draw_player_list(width, spacing);
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