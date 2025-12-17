#pragma once
#include "../BRAND.h"

#ifdef NotShxdowBrand
#else
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <ctype.h>      // toupper
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

#include <vector>
#include <string>
#include <D3DX11tex.h>
#include <d3d11.h>

#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

#define toVec4(r, g, b, a) ImColor(r / 255.0f, g / 255.0f, b / 255.0f, a)

using namespace ImGui;

enum corners {
    ROUND_ALL = 0,
    ROUND_RIGHT = 1,
    ROUND_LEFT = 2,
    ROUND_NONE = 3
};


// enums.
enum heads {
    target, sword, car, eye, people, earth, tools, scripts, settingss
};

enum subheads {
    aim, rage, other, vis1, vis2
};

// data.
inline extern bool expand = false;
inline extern float anim = 0.0f, expand_anim = 0.0f, accent_c[3] = { 32 / 255.0f, 105 / 255.0f, 244 / 255.0f };

// d3d11 textures.
inline ID3D11ShaderResourceView* valomodel = nullptr;

// defines.
#define ALPHA    (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

namespace gui {
    bool expandchildex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool expandchild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    bool expandchild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void endexpandchild();

    bool expandbutton(const char* label, bool selected);

    bool category(const char* icon, const char* label, bool selected);
    bool sub_category(const char* label, bool selected, corners which_corner_round = ROUND_ALL);

    bool beginchildex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool beginchild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    bool beginchild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void endchild();

    bool checkbox(const char* label, bool* v);

    bool sliderscalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
    bool sliderfloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
    bool sliderint(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags);

    bool begin_combo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
    void end_combo(); // only call EndCombo() if BeginCombo() returns true!
    bool combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    bool combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Three\0"
    bool combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);
    void multicombo(const char* label, bool combos[], const char* items[], int items_count);

    bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // "bool selected" carry the selection state (read-only). Selectable() is clicked is returns true so you can modify your selection state. size.x==0.0: use remaining width, size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify height
    bool selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));      // "bool* p_selected" point to the selection state (read-write), as a convenient helper.

    bool hotkey(const char* label, int* k);

    bool button_ex(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
    bool button(const char* label, const ImVec2& size = ImVec2(0, 0));

    inline void default_box(ImDrawList* draw, ImVec2 pos, ImU32 color) {
        draw->AddRect(ImVec2(pos.x + 430, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 580, pos.y + 354 + (11 * anim)), color);
    }

    inline void corners_box(ImDrawList* draw, ImVec2 pos, ImU32 color) {
        // left upper.
        draw->AddLine(ImVec2(pos.x + 430, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 480, pos.y + 114 + (11 * anim)), color);
        draw->AddLine(ImVec2(pos.x + 430, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 430, pos.y + 164 + (11 * anim)), color);

        // right upper.
        draw->AddLine(ImVec2(pos.x + 530, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 580, pos.y + 114 + (11 * anim)), color);
        draw->AddLine(ImVec2(pos.x + 580, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 580, pos.y + 165 + (11 * anim)), color);

        // left bottom.
        draw->AddLine(ImVec2(pos.x + 430, pos.y + 354 + (11 * anim)), ImVec2(pos.x + 480, pos.y + 354 + (11 * anim)), color);
        draw->AddLine(ImVec2(pos.x + 430, pos.y + 304 + (11 * anim)), ImVec2(pos.x + 430, pos.y + 354 + (11 * anim)), color);

        // right bottom.
        draw->AddLine(ImVec2(pos.x + 530, pos.y + 354 + (11 * anim)), ImVec2(pos.x + 580, pos.y + 354 + (11 * anim)), color);
        draw->AddLine(ImVec2(pos.x + 580, pos.y + 304 + (11 * anim)), ImVec2(pos.x + 580, pos.y + 354 + (11 * anim)), color);
    }


    inline void name(ImDrawList* draw, ImVec2 pos) {
#ifdef BEARUNBRAND
#else
        draw->AddText(ImVec2((pos.x + 430 + pos.x + 580) / 2 - ImGui::CalcTextSize("GULFCHEATS").x / 2, pos.y + 94 + (11 * anim)), ImColor(70, 70, 70), "GULFCHEATS");
#endif
    }

    inline void health_bar(ImDrawList* draw, ImVec2 pos) {
        // outline
        draw->AddRectFilled(ImVec2(pos.x + 584, pos.y + 113 + (11 * anim)), ImVec2(pos.x + 590, pos.y + 355 + (11 * anim)), ImColor(0, 0, 0));

        draw->AddRectFilled(ImVec2(pos.x + 585, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 589, pos.y + 354 + (11 * anim)), ImColor(0, 255, 0));
    }

    inline void distance(ImDrawList* draw, ImVec2 pos) {
        draw->AddText(ImVec2((pos.x + 430 + pos.x + 580) / 2 - ImGui::CalcTextSize("45m").x / 2, pos.y + 359 + (11 * anim)), ImColor(70, 70, 70), "45m");
    }

    inline void skeleton(ImDrawList* draw, ImVec2 pos) {
        // Adjusted positions to fit within the box
        float boxCenterX = pos.x + 505; // Center X of the box
        float boxTopY = pos.y + 120;    // Top of the box
        float boxBottomY = pos.y + 280; // Bottom of the box
        float midY = (boxTopY + boxBottomY) / 2; // Midpoint for spine

        // Short Neck
        draw->AddLine(ImVec2(boxCenterX, boxTopY + 20), ImVec2(boxCenterX, boxTopY + 30), ImColor(255, 255, 255), 2.0f);

        // Spine
        draw->AddLine(ImVec2(boxCenterX, boxTopY + 30), ImVec2(boxCenterX, midY), ImColor(255, 255, 255)); // Upper Spine
        draw->AddLine(ImVec2(boxCenterX, midY), ImVec2(boxCenterX, boxBottomY), ImColor(255, 255, 255));   // Lower Spine

        // Shoulders
        draw->AddLine(ImVec2(boxCenterX - 20, boxTopY + 30), ImVec2(boxCenterX + 20, boxTopY + 30), ImColor(255, 255, 255)); // Shoulder Line

        // Arms
        draw->AddLine(ImVec2(boxCenterX - 20, boxTopY + 30), ImVec2(boxCenterX - 40, midY), ImColor(255, 255, 255)); // Left Arm
        draw->AddLine(ImVec2(boxCenterX + 20, boxTopY + 30), ImVec2(boxCenterX + 40, midY), ImColor(255, 255, 255)); // Right Arm

        // Legs
        draw->AddLine(ImVec2(boxCenterX, boxBottomY), ImVec2(boxCenterX - 20, boxBottomY + 40), ImColor(255, 255, 255)); // Left Leg
        draw->AddLine(ImVec2(boxCenterX, boxBottomY), ImVec2(boxCenterX + 20, boxBottomY + 40), ImColor(255, 255, 255)); // Right Leg

        // Knees
        draw->AddLine(ImVec2(boxCenterX - 20, boxBottomY + 40), ImVec2(boxCenterX - 20, boxBottomY + 70), ImColor(255, 255, 255)); // Left Knee
        draw->AddLine(ImVec2(boxCenterX + 20, boxBottomY + 40), ImVec2(boxCenterX + 20, boxBottomY + 70), ImColor(255, 255, 255)); // Right Knee
    }

    inline void headcircle(ImDrawList* draw, ImVec2 pos) {
        // Adjusted positions to fit within the box
        float boxCenterX = pos.x + 505; // Center X of the box
        float boxTopY = pos.y + 120;    // Top of the box
        float boxBottomY = pos.y + 280; // Bottom of the box

        // Head
        draw->AddCircle(ImVec2(boxCenterX, boxTopY + 20), 10.0f, ImColor(255, 255, 255)); // Head circle
    }

    inline void penis(ImDrawList* draw, ImVec2 pos) {
        // Adjusted positions to place it below the spine
        float boxCenterX = pos.x + 505;  // Center X of the box
        float pelvisY = pos.y + 280;     // Bottom of the spine
        float penisEndY = pelvisY + 30;  // Length of the penis

        // Draw the penis line
        draw->AddLine(ImVec2(boxCenterX, pelvisY), ImVec2(boxCenterX, penisEndY), ImColor(255, 255, 255), 2.0f);

        // Add balls at the base
        draw->AddCircle(ImVec2(boxCenterX - 5, pelvisY + 5), 5.0f, ImColor(255, 255, 255)); // Left Ball
        draw->AddCircle(ImVec2(boxCenterX + 5, pelvisY + 5), 5.0f, ImColor(255, 255, 255)); // Right Ball
    }
}
#endif