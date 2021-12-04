#include "dependencies/std_include.hpp"
#include "friends.hpp"

namespace friends
{
	std::vector<friends_t> friends; 

	std::string get_timestamp(const std::int64_t time)
	{
		if (time)
		{
			tm ltime{};
			char timestamp[MAX_PATH] = { 0 };

			_localtime64_s(&ltime, &time);
			std::strftime(timestamp, sizeof timestamp - 1, "%D (%r)", &ltime);

			return timestamp;
		}

		return "Never";
	}
	
	void write_to_friends()
	{
		json j{};

		for (const auto& friends : friends)
		{
			j[std::to_string(friends.id)] =
			{
				{ "name", friends.name },
				{ "last_online", friends.last_online },
			};
		}

		utils::io::write_file(utils::io::get_json_file(FRIENDS_LIST), j.dump());
	}

	void refresh_friends()
	{
		friends.clear();

		if (const auto json = utils::io::parse_json_file(FRIENDS_LIST); json.is_object())
		{
			for (const auto&[key, value] : json.items())
			{
				friends.emplace_back(friends_t
				{
					std::stoull(key),
					value["name"].get<std::string>(),
					value["last_online"].get<std::int64_t>(),
				});
			}
		}
	}
	
	void remove_friend(const std::uint64_t id)
	{
		const auto entry = std::find_if(friends.begin(), friends.end(), [&id](const auto& friends) { return friends.id == id; });

		if (entry != friends.end())
		{
			friends.erase(entry);
		}

		write_to_friends();
	}

	void draw_friends_list(const float width, const float spacing)
	{
		if (ImGui::BeginTabItem("Friends"))
		{
			const auto popup{ "Add friend##add_friend_popup"s };

			static ImGuiTextFilter filter{};
			
			ImGui::TextUnformatted("Search friends");
			filter.Draw("##search_friend", "Name", width * 0.85f);

			ImGui::SameLine(0.0f, spacing);

			if (ImGui::Button("Add friend"))
			{
				ImGui::OpenPopup(popup.data());
			}

			ImGui::SetNextWindowBgAlpha(1.0f);

			if (ImGui::BeginPopupModal(popup.data(), nullptr, ImGuiWindowFlags_NoResize))
			{
				static auto name_input{ ""s };
				static auto id_input{ ""s };
				static auto ip_data_input{ ""s };

				ImGui::SetNextItemWidth(width * 0.5f);
				ImGui::InputTextWithHint("##name_input", "Name", &name_input);

				ImGui::SetNextItemWidth(width * 0.5f);
				ImGui::InputTextWithHint("##id_input", "ID", &id_input);

				if (ImGui::MenuItem("Add friend", nullptr, nullptr, !name_input.empty() && !id_input.empty()))
				{
					friends.emplace_back(friends_t{ utils::atoll(id_input), name_input });
					write_to_friends();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Refresh friends"))
				{
					refresh_friends();

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::IsKeyPressedWithIndex(ImGuiKey_Escape))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::Separator();
			
			ImGui::BeginColumns("Friends", 3, ImGuiColumnsFlags_NoResize);

			ImGui::SetColumnWidth(-1, 28.0f);
			ImGui::TextUnformatted("#");
			ImGui::NextColumn();
			ImGui::TextUnformatted("Friend");
			ImGui::NextColumn();
			ImGui::SetColumnOffset(-1, width * 0.55f);
			ImGui::TextUnformatted("Online Status");
			ImGui::NextColumn();

			ImGui::Separator();

			std::vector<std::uint32_t> indices{};

			for (auto i = 0u; i != friends.size(); ++i)
			{
				indices.emplace_back(i);
			}

			std::sort(indices.begin(), indices.end(), [](const auto& a, const auto& b) 
			{ 
				return friends[a].last_online > friends[b].last_online || friends[a].response.valid > friends[b].response.valid;
			});

			for (const auto& friend_num : indices)
			{
				auto& friends{ friends::friends[friend_num] };
				const auto response{ friends.response };
				const auto local_lobby{ response.lobby };

				if (filter.PassFilter(friends.name))
				{
					ImGui::AlignTextToFramePadding();

					ImGui::TextUnformatted(std::to_string(friend_num));

					ImGui::NextColumn();

					ImGui::PushStyleColor(ImGuiCol_Text, ImColor(200, 200, 200, 250).Value);

					ImGui::AlignTextToFramePadding();
					const auto selected{ ImGui::Selectable(friends.name.data() + "##"s + std::to_string(friend_num)) };

					ImGui::PopStyleColor();

					const auto popup{ "friend_popup##" + std::to_string(friend_num) };
					static game::netadr_t netadr{};

					if (selected)
					{
						if (!game::oob::register_remote_addr(local_lobby, &netadr))
						{
							PRINT_LOG("Failed to retrieve the remote IP address from XNADDR.");
						}
						
						ImGui::OpenPopup(popup.data());
					}

					const auto ip_str{ utils::string::adr_to_string(&netadr) }; 
					
					if (ImGui::BeginPopup(popup.data(), ImGuiWindowFlags_NoMove))
					{
						ImGui::MenuItem(friends.name + "##" + std::to_string(friend_num) + "friend_menu_item", nullptr, false, false);

						if (ImGui::IsItemClicked())
						{
							ImGui::LogToClipboardUnformatted(friends.name);
						}

						if (ImGui::IsItemHovered())
						{
							ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
						}

						if (ImGui::BeginMenu("Rename##" + std::to_string(friend_num)))
						{
							static auto rename_friend_input = ""s;

							ImGui::InputTextWithHint("##" + std::to_string(friend_num), "Name", &rename_friend_input);

							if (ImGui::MenuItem("Rename"))
							{
								if (friends.name != rename_friend_input)
								{
									friends.name = rename_friend_input;
									
									write_to_friends();
								}
							}

							ImGui::EndMenu();
						}

						if (ImGui::MenuItem("Remove"))
						{
							remove_friend(friends.id);
						}

						ImGui::Separator();

						if (ImGui::MenuItem(std::to_string(friends.id)))
						{
							ImGui::LogToClipboardUnformatted(std::to_string(friends.id));
						}

						const auto is_ready{ response.valid && netadr.inaddr }; 
						const auto ip_data_string{ !is_ready ? "Invalid IP Data" : ip_str };

						if (ImGui::MenuItem(ip_data_string, nullptr, nullptr, ip_data_string == ip_str))
						{
							ImGui::LogToClipboardUnformatted(ip_data_string);
						}

						ImGui::Separator();
						
						if (ImGui::MenuItem("Crash game", nullptr, nullptr, response.valid))
						{
							exploit::instant_message::send_info_response_overflow(friends.id);

							if (netadr.inaddr)
								exploit::send_crash(netadr);
						}
						
						if (ImGui::MenuItem("Open popup", nullptr, nullptr, response.valid))
						{
							exploit::instant_message::send_popup(friends.id);
						}

						if (ImGui::MenuItem("Kick player", nullptr, nullptr, is_ready))
						{
							exploit::send_connect_response_migration_packet(netadr);
						}

						if (ImGui::MenuItem("Show migration screen", nullptr, nullptr, is_ready))
						{
							exploit::send_mstart_packet(netadr);
						}
						
						if (ImGui::BeginMenu("Send OOB##" + std::to_string(friend_num), is_ready))
						{
							static auto oob_input = ""s;

							ImGui::InputTextWithHint("##" + std::to_string(friend_num), "OOB", &oob_input);

							if (ImGui::MenuItem("Send OOB", nullptr, nullptr, !oob_input.empty()))
							{
								game::oob::send(netadr, oob_input);
							}

							ImGui::EndMenu();
						}

						ImGui::EndPopup();
					}

					ImGui::NextColumn();

					ImGui::AlignTextToFramePadding();
					
					const auto online_status{ response.valid ? "Online" : "Last seen: " + get_timestamp(friends.last_online) };
					ImGui::TextColored(response.valid ? ImColor(0, 255, 127, 250).Value : ImColor(200, 200, 200, 250).Value, online_status.data());

					ImGui::NextColumn();

					if (ImGui::GetColumnIndex() == 0)
					{
						ImGui::Separator();
					}
				}
			}

			ImGui::EndColumns();
			ImGui::EndTabItem();
		}
	}
}
