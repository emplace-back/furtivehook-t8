#pragma once
#include "dependencies/std_include.hpp"

namespace friends
{
	struct response_t
	{
		bool valid;
		game::Msg_InfoResponse info_response;
		std::int64_t last_online;
	};

	struct friends_t
	{
		std::uint64_t id;
		std::string name;
		std::int64_t last_online;
		response_t response;
	}; 
	
	void write_to_friends();
	void refresh_friends();
	void add_friend_response(const game::Msg_InfoResponse& info_response, const std::uint64_t sender_id);
	void draw_friends_list(const float width, const float spacing);
	void initialize();

	extern std::vector<friends_t> friends;
	constexpr auto NONCE{ 1111111111u };
}