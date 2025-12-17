#include "../../BRAND.h"
#ifdef NotShxdowBrand
#include "ui.hpp"

int rotation_start_index;
void ui::ImRotateStart( ) {
    rotation_start_index = ImGui::GetWindowDrawList( )->VtxBuffer.Size;
}

ImVec2 ui::ImRotationCenter( ) {
    ImVec2 l( FLT_MAX, FLT_MAX ), u( -FLT_MAX, -FLT_MAX );

    const auto& buf = ImGui::GetWindowDrawList( )->VtxBuffer;
    for ( int i = rotation_start_index; i < buf.Size; i++ )
        l = ImMin( l, buf[i].pos ), u = ImMax( u, buf[i].pos );

    return ImVec2( ( l.x + u.x ) / 2, ( l.y + u.y ) / 2 );
}

void ui::ImRotateEnd( float rad, ImVec2 center ) {
    float s = sin( rad ), c = cos( rad );
    center = ImRotate( center, s, c ) - center;

    auto& buf = ImGui::GetWindowDrawList( )->VtxBuffer;
    for ( int i = rotation_start_index; i < buf.Size; i++ )
        buf[i].pos = ImRotate( buf[i].pos, s, c ) - center;
}

void ui::render_circle_for_horizontal_bar( ImVec2 pos, ImColor color, float alpha ) {
    auto draw = ImGui::GetWindowDrawList( );
    draw->AddCircleFilled( pos, 6, ImColor( color.Value.x, color.Value.y, color.Value.z, alpha * GImGui->Style.Alpha ) );
}

void ui::render_pages( ) {
	if ( tabs[cur_tab].pages.size( ) == 0 || tabs[cur_tab].pages.size( ) <= tabs[cur_tab].cur_subtab )
        return;

    tabs[cur_tab].pages[tabs[cur_tab].cur_subtab]( );
}

void ui::add_page( int tab_idx, std::function< void( ) > c ) {
	tabs[tab_idx].pages.emplace_back( c );
}

void ui::nav_btn( ) {
	auto window = ImGui::GetCurrentWindow( );
	auto id = window->GetID( "nav_btn" );

	ImRect rect{ window->DC.CursorPos, window->DC.CursorPos + window->Size };
	ImGui::ItemAdd( rect, id );
	ImGui::ItemSize( rect );

	bool hovered;
	bool pressed = ImGui::ButtonBehavior( rect, id, &hovered, NULL );

	window->DrawList->AddCircleFilled( rect.GetCenter( ), rect.GetWidth( ) / 2, ImColor( 18, 20, 28, int( GImGui->Style.Alpha * 255 ) ) );
	window->DrawList->AddCircle( rect.GetCenter( ), rect.GetWidth( ) / 2, ImGui::GetColorU32( ImGuiCol_Border ) );
	ImGui::RenderArrow( window->DrawList, rect.GetCenter( ) - ImVec2{ ( rect.GetWidth( ) / 1.7f ) / 2 - ( ui::nav_opened ? 0 : 3 ), ( rect.GetWidth( ) / 1.7f ) / 2 + 3 }, ImGui::GetColorU32( ImGuiCol_Text, 0.75f ), ( ui::nav_opened ? ImGuiDir_Left : ImGuiDir_Right ), rect.GetWidth( ) / 1.7f );

	if ( pressed ) ui::nav_opened = !ui::nav_opened;
}

void ui::tab( int idx ) {
	auto icon = tabs[idx].m_icon;
	auto name = tabs[idx].m_name;
	auto description = tabs[idx].m_description;

	auto window = ImGui::GetCurrentWindow( );
	auto id = window->GetID( name );

	ImRect rect{ window->DC.CursorPos, window->DC.CursorPos + ImVec2{ window->Size.x, 30 } };
	ImGui::ItemAdd( rect, id );
	ImGui::ItemSize( rect );

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior( rect, id, &hovered, &held );

	bool cur = cur_tab == idx;

	float anim = ImTricks::Animations::FastFloatLerp( name, 0, cur, 0.f, 1.f, anim_speed );
	float nav_anim = ImTricks::Animations::FastFloatLerp( name, 1, ui::nav_opened, 0.f, 1.f, anim_speed );
	float hovered_anim = ImTricks::Animations::FastFloatLerp( name, 2, hovered || cur, 0.f, 1.f, anim_speed );
	auto icon_col = ImTricks::Animations::FastColorLerp( ImGui::GetColorU32( ImGuiCol_TextDisabled ), ImGui::GetColorU32( ImGuiCol_Scheme ), anim );
	auto text_col = ImTricks::Animations::FastColorLerp( ImGui::GetColorU32( ImGuiCol_TextDisabled ), ImGui::GetColorU32( ImGuiCol_Text ), hovered_anim );
	auto description_col = ImTricks::Animations::FastColorLerp( ImGui::GetColorU32( ImGuiCol_TextDisabled, 0.5f ), ImGui::GetColorU32( ImGuiCol_TextDisabled ), hovered_anim );

	glow::rect_glow( { rect.Min.x + 24, rect.GetCenter( ).y - ImGui::CalcTextSize( icon ).y / 2 - 3 }, { rect.Min.x + 24 + ImGui::CalcTextSize( icon ).x, rect.GetCenter( ).y + ImGui::CalcTextSize( icon ).y / 2 }, ImColor( icon_col.Value.x, icon_col.Value.y, icon_col.Value.z, icon_col.Value.w * ( 0.06f * anim ) ), 10, 4 );
	window->DrawList->AddText( { rect.Min.x + 24, rect.GetCenter( ).y - ImGui::CalcTextSize( icon ).y / 2 }, icon_col, icon );
	window->DrawList->AddText( font( 1 ), font( 1 )->FontSize, { rect.Min.x + 55, rect.Min.y }, ImColor( text_col.Value.x, text_col.Value.y, text_col.Value.z, text_col.Value.w * nav_anim ), name, ImGui::FindRenderedTextEnd( name ) );
	window->DrawList->AddText( font( 2 ), font( 2 )->FontSize, { rect.Min.x + 55, rect.Max.y - font( 2 )->CalcTextSizeA( font( 2 )->FontSize, FLT_MAX, 0.f, description, ImGui::FindRenderedTextEnd( description ) ).y }, ImColor( description_col.Value.x, description_col.Value.y, description_col.Value.z, description_col.Value.w * nav_anim ), description, ImGui::FindRenderedTextEnd( description ) );

	if ( pressed ) cur_tab = idx, ui::anim = 0.f;
}

void ui::separator( float x_padding, float sz ) {
	auto window = ImGui::GetCurrentWindow( );

	ImRect rect{ window->DC.CursorPos + ImVec2{ x_padding, 0 }, window->DC.CursorPos + ImVec2{ sz > 0.f ? sz : window->Size.x - x_padding, 1 } };
	ImGui::ItemAdd( rect, 0 );
	ImGui::ItemSize( rect );

	window->DrawList->AddRectFilled( rect.Min, rect.Max, ImColor( 1.f, 1.f, 1.f, 0.04f * GImGui->Style.Alpha ) );
}

void ui::vertical_separator( float y_padding, float sz ) {
	auto window = ImGui::GetCurrentWindow( );

	ImRect rect{ window->DC.CursorPos + ImVec2{ 0, y_padding }, window->DC.CursorPos + ImVec2{ 1, sz > 0.f ? sz : window->Size.y - y_padding } };
	ImGui::ItemAdd( rect, 0 );
	ImGui::ItemSize( rect );

	window->DrawList->AddRectFilled( rect.Min, rect.Max, ImColor( 1.f, 1.f, 1.f, 0.04f * GImGui->Style.Alpha ) );
}

void ui::subtab( int idx ) {
	auto name = tabs[ui::cur_tab].m_subtabs[idx];

	auto window = ImGui::GetCurrentWindow( );
	auto id = window->GetID( name );

	bool cur = tabs[ui::cur_tab].cur_subtab == idx;

	float anim = ImTricks::Animations::FastFloatLerp( name, 0, cur, 0.f, 1.f, anim_speed );

	ImRect rect{ window->DC.CursorPos, window->DC.CursorPos + font( 3 )->CalcTextSizeA( font( 3 )->FontSize, FLT_MAX, 0.f, name, ImGui::FindRenderedTextEnd( name ) ) + ImVec2{ 14 * anim, 0 } };
	ImGui::ItemAdd( rect, id );
	ImGui::ItemSize( rect );

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior( rect, id, &hovered, &held );

	float hovered_anim = ImTricks::Animations::FastFloatLerp( name, 1, hovered || cur, 0.f, 1.f, anim_speed );
	auto text_col = ImTricks::Animations::FastColorLerp( ImGui::GetColorU32( ImGuiCol_TextDisabled ), ImGui::GetColorU32( ImGuiCol_Text ), hovered_anim );

	glow::circle_filled_glow( { rect.Min.x + 3, rect.GetCenter( ).y }, 3 * anim, ImGui::GetColorU32( ImGuiCol_Scheme, anim * 0.05f ), 4 );
	window->DrawList->AddCircleFilled( { rect.Min.x + 3, rect.GetCenter( ).y }, 3 * anim, ImGui::GetColorU32( ImGuiCol_Scheme, anim ) );
	window->DrawList->AddText( font( 3 ), font( 3 )->FontSize, { rect.Max.x - font( 3 )->CalcTextSizeA( font( 3 )->FontSize, FLT_MAX, 0.f, name, ImGui::FindRenderedTextEnd( name ) ).x, rect.Min.y }, text_col, name, ImGui::FindRenderedTextEnd( name ) );

	if ( pressed ) tabs[ui::cur_tab].cur_subtab = idx, ui::anim = 0.f;
}

void ui::begin_child( const char* name, ImVec2 size ) {
    ImGui::BeginChild( std::string( name ).append( "##decoration" ).c_str( ), size, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar );
    ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_ChildBg ), ImGui::GetStyle( ).ChildRounding );
	ImGui::GetWindowDrawList( )->AddRect( ImGui::GetWindowPos( ), ImGui::GetWindowPos( ) + ImGui::GetWindowSize( ), ImGui::GetColorU32( ImGuiCol_Border ), ImGui::GetStyle( ).ChildRounding, 0, ImGui::GetStyle( ).ChildBorderSize );
	ImGui::GetWindowDrawList( )->AddRectFilled( ImGui::GetWindowPos( ) + ImVec2{ 0, 10 }, ImGui::GetWindowPos( ) + ImVec2{ ImGui::GetWindowWidth( ), 40 }, ImGui::GetColorU32( ImGuiCol_Header ) );
	ImGui::GetWindowDrawList( )->AddLine( ImGui::GetWindowPos( ) + ImVec2{ 0, 10 }, ImGui::GetWindowPos( ) + ImVec2{ ImGui::GetWindowWidth( ), 10 }, ImGui::GetColorU32( ImGuiCol_Border ) );
	ImGui::GetWindowDrawList( )->AddLine( ImGui::GetWindowPos( ) + ImVec2{ 0, 39 }, ImGui::GetWindowPos( ) + ImVec2{ ImGui::GetWindowWidth( ), 39 }, ImGui::GetColorU32( ImGuiCol_Border ) );
	ImGui::GetWindowDrawList( )->AddText( font( 1 ), font( 1 )->FontSize, ImGui::GetWindowPos( ) + ImVec2{ 14, 18 }, ImGui::GetColorU32( ImGuiCol_Text ), name, ImGui::FindRenderedTextEnd( name ) );

    ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, { 14, 10 } );
    ImGui::SetCursorPosY( 41 );
    ImGui::BeginChild( name, { size.x, size.y - 42 }, 0, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoBackground );

    ImGui::BeginGroup( );
    ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 8, 10 } );
}

void ui::end_child( ) {
    ImGui::PopStyleVar( 2 );
    ImGui::EndGroup( );
    ImGui::EndChild( );
    ImGui::EndChild( );
}

void ui::slider_int( const char* name, int* v, int min, int max ) {
    auto window = ImGui::GetCurrentWindow( );
	auto id = window->GetID( name );
	auto style = ImGui::GetStyle( );
	auto g = *GImGui;

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 name_size = ImGui::CalcTextSize( name, 0, 1 );

	ImRect bb( pos + ImVec2( window->Size.x - style.WindowPadding.x * 2 - 120, 4 ), pos + ImVec2( window->Size.x - style.WindowPadding.x * 2, 11 ) );
	ImRect total_bb( pos, bb.Max );
	ImGui::ItemAdd( total_bb, id );
	ImGui::ItemSize( total_bb );

	const bool hovered = ImGui::ItemHoverable( bb, id, 0 );

    static std::unordered_map< const char*, bool > active_map;
    auto active = active_map.find( name );

    if ( active == active_map.end( ) ) {
        active_map.insert( { name, false } );
        active = active_map.find( name );
    }

    if ( hovered && ImGui::IsMouseClicked( 0 ) )
        active->second = true;

    if ( active->second ) {
        *v = ImClamp( int( ( ImGui::GetIO( ).MousePos.x - bb.Min.x ) / bb.GetWidth( ) * max ), min, max );
    }

    if ( !ImGui::IsMouseDown( 0 ) ) active->second = false;

    static std::unordered_map< ImGuiID, float > values;
    auto value = values.find( id );

    if ( value == values.end( ) ) {
        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, bb.GetWidth( ) * ( *v ) / max, anim_speed * 2 );

    float anim = ImTricks::Animations::FastFloatLerp( name, 0, hovered || active->second, 0.f, 1.f, anim_speed );
    auto col = ImTricks::Animations::FastColorLerp( ImGui::GetColorU32( ImGuiCol_Border ), ImGui::GetColorU32( ImGuiCol_Scheme, 0.5f ), anim );

	window->DrawList->AddRectFilled( bb.Min, bb.Max, ImGui::GetColorU32( ImGuiCol_FrameBg ), 3 );
    window->DrawList->AddRect( bb.Min, bb.Max, col, 3 );
	glow::rect_glow( bb.Min, { bb.Min.x + value->second, bb.Max.y }, ImGui::GetColorU32( ImGuiCol_Scheme, ( value->second > 0.01f ) ? 0.08f : 0.f ), 10, 4 );
    window->DrawList->AddRectFilled( bb.Min, { bb.Min.x + value->second, bb.Max.y }, ImGui::GetColorU32( ImGuiCol_Scheme, ( value->second > 0.01f ) ), 3 );
	window->DrawList->AddCircleFilled( { bb.Min.x + value->second, bb.GetCenter( ).y }, 7 - 1 * anim, ImGui::GetColorU32( ImGuiCol_CheckMark ), 128 );

    ImGui::GetForegroundDrawList( )->AddRectFilled( { bb.Min.x + value->second - ImGui::CalcTextSize( std::to_string( *v ).c_str( ) ).x / 2 - 4, bb.Min.y - 12 - ImGui::CalcTextSize( std::to_string( *v ).c_str( ) ).y }, { bb.Min.x + value->second + ImGui::CalcTextSize( std::to_string( *v ).c_str( ) ).x / 2 + 4, bb.Min.y - 7 }, ImColor( 26, 28, 36, int( ( style.Alpha * anim ) * 255.f ) ), 3 );
	ImGui::GetForegroundDrawList( )->AddText( { bb.Min.x + value->second - ImGui::CalcTextSize( std::to_string( *v ).c_str( ) ).x / 2, bb.Min.y - 10 - ImGui::CalcTextSize( std::to_string( *v ).c_str( ) ).y }, ImGui::GetColorU32( ImGuiCol_Text, anim ), std::to_string( *v ).c_str( ) );

    ImGui::RenderText( total_bb.Min, name );
}

void ui::slider_float( const char* name, float* v, float min, float max ) {
    auto window = ImGui::GetCurrentWindow( );
	auto id = window->GetID( name );
	auto style = ImGui::GetStyle( );
	auto g = *GImGui;

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 name_size = ImGui::CalcTextSize( name, 0, 1 );

	ImRect bb( pos + ImVec2( window->Size.x - style.WindowPadding.x * 2 - 120, 4 ), pos + ImVec2( window->Size.x - style.WindowPadding.x * 2, 11 ) );
	ImRect total_bb( pos, bb.Max );
	ImGui::ItemAdd( total_bb, id );
	ImGui::ItemSize( total_bb );
    
	const bool hovered = ImGui::ItemHoverable( bb, id, 0 );

    static std::unordered_map< const char*, bool > active_map;
    auto active = active_map.find( name );

    if ( active == active_map.end( ) ) {
        active_map.insert( { name, false } );
        active = active_map.find( name );
    }

    if ( hovered && ImGui::IsMouseClicked( 0 ) )
        active->second = true;

    if ( active->second ) {
        *v = ImClamp( float( ( ImGui::GetIO( ).MousePos.x - bb.Min.x ) / bb.GetWidth( ) * max ), min, max );
    }

    if ( !ImGui::IsMouseDown( 0 ) ) active->second = false;

    static std::unordered_map< ImGuiID, float > values;
    auto value = values.find( id );

    if ( value == values.end( ) ) {
        values.insert( { id, 0.f } );
        value = values.find( id );
    }

    value->second = ImLerp( value->second, bb.GetWidth( ) * ( *v ) / max, anim_speed * 2 );

	float anim = ImTricks::Animations::FastFloatLerp( name, 0, hovered || active->second, 0.f, 1.f, anim_speed );
    auto col = ImTricks::Animations::FastColorLerp( ImGui::GetColorU32( ImGuiCol_Border ), ImGui::GetColorU32( ImGuiCol_Scheme, 0.5f ), anim );

	window->DrawList->AddRectFilled( bb.Min, bb.Max, ImGui::GetColorU32( ImGuiCol_FrameBg ), 3 );
    window->DrawList->AddRect( bb.Min, bb.Max, col, 3 );
	glow::rect_glow( bb.Min, { bb.Min.x + value->second, bb.Max.y }, ImGui::GetColorU32( ImGuiCol_Scheme, ( value->second > 0.01f ) ? 0.08f : 0.f ), 10, 4 );
    window->DrawList->AddRectFilled( bb.Min, { bb.Min.x + value->second, bb.Max.y }, ImGui::GetColorU32( ImGuiCol_Scheme, ( value->second > 0.01f ) ), 3 );
	window->DrawList->AddCircleFilled( { bb.Min.x + value->second, bb.GetCenter( ).y }, 7 - 1 * anim, ImGui::GetColorU32( ImGuiCol_CheckMark ), 128 );

	char to_print[64];
	sprintf_s( to_print, "%.2f", *v );

    ImGui::GetForegroundDrawList( )->AddRectFilled( { bb.Min.x + value->second - ImGui::CalcTextSize( to_print ).x / 2 - 4, bb.Min.y - 12 - ImGui::CalcTextSize( to_print ).y }, { bb.Min.x + value->second + ImGui::CalcTextSize( to_print ).x / 2 + 4, bb.Min.y - 7 }, ImColor( 26, 28, 36, int( ( style.Alpha * anim ) * 255.f ) ), 3 );
	ImGui::GetForegroundDrawList( )->AddText( { bb.Min.x + value->second - ImGui::CalcTextSize( to_print ).x / 2, bb.Min.y - 10 - ImGui::CalcTextSize( to_print ).y }, ImGui::GetColorU32( ImGuiCol_Text, anim ), to_print );

    ImGui::RenderText( total_bb.Min, name );
}
#else
#endif