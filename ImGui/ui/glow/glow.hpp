#pragma once

#include "../../../BRAND.h"
#ifdef NotShxdowBrand

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../../notshxdow/imgui_1.h"
#include "../../notshxdow/imgui_internal_1.h"

namespace glow {
	inline void text_glow( ImFont* font, float font_size, ImVec2 pos, ImColor color, const char* text, ImDrawList* draw = ImGui::GetWindowDrawList( ) ) {
		int f = 15;

		for ( int i = 0; i < f; ++i ) {
			draw->AddText( font, font_size + ( f - i ), { pos.x + font->CalcTextSizeA( font_size, FLT_MAX, -1, text ).x / 2 - font->CalcTextSizeA( font_size + ( f - i ), FLT_MAX, -1, text ).x / 2, pos.y + font->CalcTextSizeA( font_size, FLT_MAX, -1, text ).y / 2 - font->CalcTextSizeA( font_size + ( f - i ), FLT_MAX, -1, text ).y / 2 }, ImColor{ color.Value.x, color.Value.y, color.Value.z, 0.15f / ( f - i ) * color.Value.w }, text );
		}
	}

	inline void rect_glow( ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity, ImDrawList* draw = ImGui::GetWindowDrawList( ) ) {
		while ( true ) {
			if ( col.Value.w < 0.0019f )
				break;

			draw->AddRectFilled( start, end, col, rounding );

			col.Value.w -= col.Value.w / intensity;
			start = ImVec2( start.x - 1, start.y - 1 );
			end = ImVec2( end.x + 1, end.y + 1 );
		}
	}

	inline void circle_filled_glow( ImVec2 center, float radius, ImColor col, float intensity, ImDrawList* draw = ImGui::GetWindowDrawList( ) ) {
		while ( true ) {
			if ( col.Value.w < 0.0019f )
				break;

			draw->AddCircleFilled( center, radius, col, 24 );

			col.Value.w -= col.Value.w / intensity;
			radius += 2;
		}
	}
}
#else
#endif