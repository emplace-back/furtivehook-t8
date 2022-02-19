#pragma once
#include "dependencies/std_include.hpp"

namespace menu
{
	constexpr static auto window_title = "furtivehook"; 
	constexpr static auto color_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip;
	constexpr static auto window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	extern bool initialized;
	extern ImFont* glacial_indifference_bold;
	extern ImFont* glacial_indifference;

	void set_style();
	bool is_open();
	bool begin_section(const std::string& text);
	void draw();
	void initialize(IDXGISwapChain * swap_chain);
}