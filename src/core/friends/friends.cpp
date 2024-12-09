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

	void refresh_friend_online_status()
	{
		std::vector<std::uint64_t> recipients{};

		for (const auto& friends : friends::friends)
		{
			recipients.emplace_back(friends.id);
		}

		utils::for_each_batch<std::uint64_t>(recipients, 18, [](const auto& ids)
		{
			events::instant_message::send_info_request(ids);
		});
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

		refresh_friend_online_status();
	}

	void add_friend_response(const game::Msg_InfoResponse& info_response, friends::friends_t& friends)
	{
		response_t response{};
		response.valid = true;
		response.info_response = info_response;
		response.last_online = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		
		if (!friends.response.valid)
		{
			friends.last_online = response.last_online;
			friends::write_to_friends();

			PRINT_MESSAGE("Friends", "%s is online.", friends.name.data()); 
		}

		friends.response = response;
	}

	void add_friend_response(const game::Msg_InfoResponse& info_response, const std::uint64_t sender_id)
	{
		for (auto& friends : friends::friends)
		{
			if (friends.id == sender_id)
			{
				add_friend_response(info_response, friends);
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

			ImGui::SetColumnWidth(-1, 38.0f);
			ImGui::TextUnformatted("#");
			ImGui::NextColumn();
			ImGui::TextUnformatted("Friend");
			ImGui::NextColumn();
			ImGui::SetColumnOffset(-1, width * 0.55f);
			ImGui::TextUnformatted("Online Status");
			ImGui::NextColumn();

			ImGui::Separator();

			std::vector<size_t> indices{};

			for (size_t i = 0; i < friends.size(); ++i)
			{
				indices.emplace_back(i);
			}

			std::sort(indices.begin(), indices.end(), [](const auto& a, const auto& b) 
			{ 
				return friends[a].last_online > friends[b].last_online || friends[a].response.valid > friends[b].response.valid;
			});

			for (const auto& friend_num : indices)
			{
				if (auto& friends{ friends::friends[friend_num] }; filter.PassFilter(friends.name))
				{
					const auto response{ friends.response };
					const auto lobby{ response.info_response.lobby[0] };
					
					ImGui::AlignTextToFramePadding();

					ImGui::TextUnformatted(std::to_string(friend_num));

					ImGui::NextColumn();

					ImGui::PushStyleColor(ImGuiCol_Text, ImColor(200, 200, 200, 250).Value);

					ImGui::AlignTextToFramePadding();
					const auto selected{ ImGui::Selectable(friends.name.data() + "##"s + std::to_string(friend_num)) };

					ImGui::PopStyleColor();

					const auto is_local{ lobby.hostXuid == friends.id && lobby.isValid };

					if (is_local && lobby.hostName != friends.name)
					{
						ImGui::SameLine(0, spacing);
						ImGui::TextColored(ImColor(200, 200, 200, 250).Value, "(%s)", lobby.hostName);
					}

					const auto popup{ "friend_popup##" + std::to_string(friend_num) };
					static game::netadr_t netadr{};
					
					if (selected)
					{
						if (!game::oob::register_remote_addr(lobby, &netadr))
						{
							PRINT_LOG("Failed to retrieve the remote IP address from XNADDR.");
						}
						
						ImGui::OpenPopup(popup.data());
					}
					
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

						const auto is_ready{ response.valid && netadr.inaddr && netadr.type != game::NA_BAD };
						const auto ip_data_string{ is_ready ? utils::string::adr_to_string(&netadr) : "Invalid IP Data" };

						if (ImGui::MenuItem(ip_data_string, nullptr, nullptr, is_ready))
						{
							ImGui::LogToClipboardUnformatted(ip_data_string);
						}

						if (is_ready)
						{
							ImGui::SameLine();
							ImGui::TextColored(ImColor(200, 200, 200, 250).Value, is_local ? "(Local)" : "(Session)");
						}

						ImGui::Separator();
						
						if (ImGui::MenuItem("Crash game", nullptr, nullptr, response.valid))
						{
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

						if (ImGui::MenuItem("Remove from party", nullptr, nullptr, is_ready))
						{
							exploit::lobby_msg::send_disconnect_client(netadr, friends.id);
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

	void initialize()
	{
		scheduler::on_dw_initialized(refresh_friend_online_status, scheduler::pipeline::backend, 60s);
	}
}
