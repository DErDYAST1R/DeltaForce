#pragma once

#include "../../BRAND.h"
#ifdef NotShxdowBrand

#define IMGUI_DEFINE_MATH_OPERATORS

#include "../notshxdow/imgui_1.h"
#include "../notshxdow/imgui_internal_1.h"
#include "../notshxdow/imgui_impl_dx11_1.h"
#include "../notshxdow/imgui_impl_win32_1.h"
#include "tricks/imgui_tricks.hpp"
#include "glow/glow.hpp"
#include "hashes.hpp"
#include "bytes.hpp"

#include <functional>

#include <d3d11.h>
#include <d3dx11.h>
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx11.lib" )

#define font( n ) ImGui::GetIO( ).Fonts->Fonts[ n ]

#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
#define NO_ALPHA ( ImGuiColorEditFlags_NoTooltip    | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )

struct tab_t {
	const char* m_icon,
		*m_name, 
		*m_description;

	std::vector< std::function< void( ) > > pages;
	std::vector< const char* > m_subtabs;
	int cur_subtab = 0;

	tab_t( const char* icon, const char* name, const char* description, std::vector< const char* > subtabs ) : m_icon{ icon }, m_name{ name }, m_description{ description }, 
		m_subtabs{ subtabs } { };
};

namespace ui {
	inline ImVec2 size{ 650, 519 };
	inline float accent_col_holder[4] = { 1.f, 1.f, 1.f, 1.f };

	inline float anim = 0.f, anim_speed = 0.125f;

	inline bool nav_opened = false;
	inline float nav_width = 66.f;

	void ImRotateStart( );
	ImVec2 ImRotationCenter( );
	void ImRotateEnd( float rad, ImVec2 center = ImRotationCenter( ) );

	void render_circle_for_horizontal_bar( ImVec2 pos, ImColor color, float alpha );

	void add_page( int tab_idx, std::function< void( ) > c );
	void render_pages( );

	void nav_btn( );

	inline int cur_tab = 0;
	inline std::vector< tab_t > tabs = { 
		{ "AIM", "Aimbot", "Aimbot", { "Aimbot" } },
		{ "ESP", "Visuals", "ESP", { "Visuals" } },
		{ "MISC", "Misc", "Other", { "Miscellaneous" } } };

	/*
		{ "AIM", "Aimbot", "Shooting assistance", { "Subtab", "Subtab 1", "Subtab 2" } },
		{ "ESP", "Visuals", "ESP, terrain", { "ESP", "Terrain" } },
		{ "MISC", "Misc.", "Configs, other", { } } };
*/

	void tab( int idx );
	void separator( float x_padding = 0.f, float sz = 0.f );
	void vertical_separator( float y_padding = 0.f, float sz = 0.f );
	void subtab( int idx );

	void begin_child( const char* name, ImVec2 size );
	void end_child( );

	void slider_int( const char* name, int* v, int min, int max );
	void slider_float( const char* name, float* v, float min, float max );
}
#else
#endif