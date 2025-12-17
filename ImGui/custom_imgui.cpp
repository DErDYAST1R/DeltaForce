#include "../BRAND.h"
#ifdef NotShxdowBrand
#else
#include "custom_imgui.h"

bool gui::expandchildex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;

    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

    // Size
    const ImVec2 content_avail = GetContentRegionAvail();
    ImVec2 size = ImFloor(size_arg);
    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y + size.y, 4.0f);
    SetNextWindowSize(size);

    // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
    const char* temp_window_name;
    if (name)
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
    else
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

    if (expand) {
        GetForegroundDrawList()->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + size, ImColor(254, 254, 254), g.Style.WindowRounding, ImDrawFlags_RoundCornersLeft);
        GetForegroundDrawList()->AddLine(ImVec2(parent_window->DC.CursorPos.x + size.x, parent_window->DC.CursorPos.y), parent_window->DC.CursorPos + size, ImColor(0.0f, 0.0f, 0.0f, 0.10f));
    }
    else {
        parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + size, ImColor(254, 254, 254), g.Style.WindowRounding, ImDrawFlags_RoundCornersLeft);
        parent_window->DrawList->AddLine(ImVec2(parent_window->DC.CursorPos.x + size.x, parent_window->DC.CursorPos.y), parent_window->DC.CursorPos + size, ImColor(0.0f, 0.0f, 0.0f, 0.10f));
    }

    const float backup_border_size = g.Style.ChildBorderSize;
    g.Style.ChildBorderSize = 0.0f;
    bool ret = Begin(temp_window_name, NULL, flags);
    g.Style.ChildBorderSize = backup_border_size;

    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;
    child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

    // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
    // While this is not really documented/defined, it seems that the expected thing to do.
    if (child_window->BeginCount == 1)
        parent_window->DC.CursorPos = child_window->Pos;

    // Process navigation-in immediately so NavInit can run on first frame
    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
        g.ActiveIdSource = ImGuiInputSource_Nav;
    }
    return ret;
}

bool gui::expandchild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    return expandchildex(str_id, window->GetID(str_id), size_arg, border, extra_flags);
}

bool gui::expandchild(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    IM_ASSERT(id != 0);
    return expandchildex(NULL, id, size_arg, border, extra_flags);
}

void gui::endexpandchild()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(g.WithinEndChild == false);
    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

    g.WithinEndChild = true;
    if (window->BeginCount > 1)
    {
        End();
    }
    else
    {
        ImVec2 sz = window->Size;
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
            sz.x = ImMax(4.0f, sz.x);
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
            sz.y = ImMax(4.0f, sz.y);
        End();

        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
        ItemSize(sz);
        if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
        {
            ItemAdd(bb, window->ChildId);
            RenderNavHighlight(bb, window->ChildId);

            // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
            if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
                RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
        }
        else
        {
            // Not navigable into
            ItemAdd(bb, 0);
        }
        if (g.HoveredWindow == window)
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
    }
    g.WithinEndChild = false;
    g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
}

struct expand_struct {
    float transparency, transparency_inversed;
};

bool gui::expandbutton(const char* label, bool selected)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = io.Fonts->Fonts[3]->CalcTextSizeA(20.0f, FLT_MAX, 0.0f, label);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x, pos.y + label_size.y));
    ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y), style.FramePadding.y);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, expand_struct> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f, 0.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.transparency = ImLerp(it_animation->second.transparency, (selected ? 1.0f : 0.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));
    it_animation->second.transparency_inversed = ImLerp(it_animation->second.transparency_inversed, (selected ? 0.0f : 1.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));

    if (expand) {
        GetForegroundDrawList()->AddText(io.Fonts->Fonts[3], 20.0f, (rect.Min + rect.Max) / 2 - (label_size) / 2, toVec4(110, 110, 110, it_animation->second.transparency_inversed), label);
        GetForegroundDrawList()->AddText(io.Fonts->Fonts[3], 20.0f, (rect.Min + rect.Max) / 2 - (label_size) / 2, ImColor(accent_c[0], accent_c[1], accent_c[2], it_animation->second.transparency), label);
    }
    else {
        window->DrawList->AddText(io.Fonts->Fonts[3], 20.0f, (rect.Min + rect.Max) / 2 - (label_size) / 2, toVec4(110, 110, 110, it_animation->second.transparency_inversed), label);
        window->DrawList->AddText(io.Fonts->Fonts[3], 20.0f, (rect.Min + rect.Max) / 2 - (label_size) / 2, ImColor(accent_c[0], accent_c[1], accent_c[2], it_animation->second.transparency), label);
    }

    return pressed;
}

struct category_struct {
    float transparency, transparency_inversed, move;
};

bool gui::category(const char* icon, const char* label, bool selected)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(icon);
    const ImVec2 icon_size = io.Fonts->Fonts[1]->CalcTextSizeA(12.0f, FLT_MAX, 0.0f, icon);
    const ImVec2 label_size = CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 26 + ((label_size.x + 22) * expand_anim), pos.y + 25));
    ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, category_struct> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.transparency = ImLerp(it_animation->second.transparency, (selected ? 1.0f : 0.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));
    it_animation->second.transparency_inversed = ImLerp(it_animation->second.transparency_inversed, (selected ? 0.0f : 1.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));

    it_animation->second.move = ImLerp(it_animation->second.move, (selected ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));

    if (expand) {
        if (expand_anim >= 0.90f) {
            GetForegroundDrawList()->AddRectFilled(ImVec2(rect.Min.x - 37, rect.Min.y + 3 * it_animation->second.move), ImVec2(rect.Min.x - 35, rect.Max.y - 3 * it_animation->second.move), ImColor(accent_c[0], accent_c[1], accent_c[2], it_animation->second.move));
            GetForegroundDrawList()->AddRectFilledMultiColor(ImVec2(rect.Min.x - 37, rect.Min.y + 3 * it_animation->second.move), ImVec2(rect.Min.x - 21, rect.Max.y - 3 * it_animation->second.move), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.45f * it_animation->second.move), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.0f), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.0f), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.45f * it_animation->second.move));
        }

        GetForegroundDrawList()->AddShadowCircle((rect.Min + rect.Max) / 2, 3.0f, ImColor(1.0f, 1.0f, 1.0f, 0.15f * it_animation->second.transparency_inversed), 60.0f, ImVec2(0, 0));
        GetForegroundDrawList()->AddShadowCircle((rect.Min + rect.Max) / 2, 10.0f, ImColor(accent_c[0], accent_c[1], accent_c[2], 0.5f * it_animation->second.transparency), 60.0f, ImVec2(0, 0));

        GetForegroundDrawList()->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, 0.01f * it_animation->second.transparency), 3.0f);
        GetForegroundDrawList()->AddRect(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, 0.10f * it_animation->second.transparency), 3.0f);

        GetForegroundDrawList()->AddText(io.Fonts->Fonts[1], 12.0f, ImVec2(rect.Min.x + 10, (rect.Min.y + rect.Max.y) / 2 - icon_size.y / 2), toVec4(70, 70, 70, 0.35f * it_animation->second.transparency_inversed), icon);
        GetForegroundDrawList()->AddText(io.Fonts->Fonts[1], 12.0f, ImVec2(rect.Min.x + 10, (rect.Min.y + rect.Max.y) / 2 - icon_size.y / 2), ImColor(accent_c[0], accent_c[1], accent_c[2], it_animation->second.transparency), icon);

        GetForegroundDrawList()->AddText((rect.Min + rect.Max) / 2 - label_size / 2 + ImVec2(9, 0), toVec4(70, 70, 70, selected ? 1.0f : 0.40f), label);
    }
    else {
        window->DrawList->AddRectFilled(ImVec2(rect.Min.x - 21, rect.Min.y + 3 * it_animation->second.move), ImVec2(rect.Min.x - 19, rect.Max.y - 3 * it_animation->second.move), ImColor(accent_c[0], accent_c[1], accent_c[2], it_animation->second.move));
        window->DrawList->AddRectFilledMultiColor(ImVec2(rect.Min.x - 21, rect.Min.y + 3 * it_animation->second.move), ImVec2(rect.Min.x - 10, rect.Max.y - 3 * it_animation->second.move), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.45f * it_animation->second.move), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.0f), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.0f), ImColor(accent_c[0], accent_c[1], accent_c[2], 0.45f * it_animation->second.move));

        window->DrawList->AddShadowCircle((rect.Min + rect.Max) / 2, 3.0f, ImColor(1.0f, 1.0f, 1.0f, 0.15f * it_animation->second.transparency_inversed), 60.0f, ImVec2(0, 0));
        window->DrawList->AddShadowCircle((rect.Min + rect.Max) / 2, 10.0f, ImColor(accent_c[0], accent_c[1], accent_c[2], 0.5f * it_animation->second.transparency), 60.0f, ImVec2(0, 0));

        window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, 0.01f * it_animation->second.transparency), 3.0f);
        window->DrawList->AddRect(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, 0.10f * it_animation->second.transparency), 3.0f);

        window->DrawList->AddText(io.Fonts->Fonts[1], 12.0f, (rect.Min + rect.Max) / 2 - (icon_size) / 2, toVec4(70, 70, 70, 0.35f * it_animation->second.transparency_inversed), icon);
        window->DrawList->AddText(io.Fonts->Fonts[1], 12.0f, (rect.Min + rect.Max) / 2 - (icon_size) / 2, ImColor(accent_c[0], accent_c[1], accent_c[2], it_animation->second.transparency), icon);
    }

    if (pressed)
        expand = false;

    return pressed;
}

struct sub_category_struct {
    float transparency, text_transparency;
};

bool gui::sub_category(const char* label, bool selected, corners which_corner_round)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 100, pos.y + 27));
    ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x - 9, rect.Max.y + 5), style.FramePadding.y);
    if (!ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, sub_category_struct> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f, 0.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.transparency = ImLerp(it_animation->second.transparency, (selected ? 1.0f : 0.0f), 0.10f * (1.0f - ImGui::GetIO().DeltaTime));
    it_animation->second.text_transparency = ImLerp(it_animation->second.text_transparency, (selected ? 1.0f : 0.7f), 0.10f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(225, 226, 230), 3.0f, (which_corner_round == ROUND_RIGHT ? ImDrawFlags_RoundCornersRight : which_corner_round == ROUND_LEFT ? ImDrawFlags_RoundCornersLeft : which_corner_round == ROUND_ALL ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersNone));
    window->DrawList->AddRectFilled(rect.Min, rect.Max, toVec4(205, 206, 210, it_animation->second.transparency), 3.0f, (which_corner_round == ROUND_RIGHT ? ImDrawFlags_RoundCornersRight : which_corner_round == ROUND_LEFT ? ImDrawFlags_RoundCornersLeft : which_corner_round == ROUND_ALL ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersNone));

    window->DrawList->AddRect(rect.Min, rect.Max, ImColor(0.0f, 0.0f, 0.0f, 0.10f), 3.0f, (which_corner_round == ROUND_RIGHT ? ImDrawFlags_RoundCornersRight : which_corner_round == ROUND_LEFT ? ImDrawFlags_RoundCornersLeft : which_corner_round == ROUND_ALL ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersNone));

    window->DrawList->AddText((rect.Min + rect.Max) / 2 - (label_size) / 2, toVec4(70, 70, 70, it_animation->second.text_transparency), label);

    return pressed;
}

bool gui::beginchildex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;

    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag

    // Size
    const ImVec2 content_avail = GetContentRegionAvail();
    ImVec2 size = ImFloor(size_arg);
    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y + size.y, 4.0f);

    SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 28.0f));
    SetNextWindowSize(size - ImVec2(0, 30));

    parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(254, 254, 254), 4.0f);
    parent_window->DrawList->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(0.0f, 0.0f, 0.0f, 0.15f), 4.0f);

    parent_window->DrawList->AddLine(parent_window->DC.CursorPos + ImVec2(1, 27), parent_window->DC.CursorPos + ImVec2(size.x - 1, 27), ImColor(0.0f, 0.0f, 0.0f, 0.15f));

    parent_window->DrawList->AddText(parent_window->DC.CursorPos + ImVec2(11, 7), ImColor(70, 70, 70), name);

    // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
    const char* temp_window_name;
    if (name)
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
    else
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

    const float backup_border_size = g.Style.ChildBorderSize;
    if (!border)
        g.Style.ChildBorderSize = 0.0f;
    bool ret = Begin(temp_window_name, NULL, flags);
    g.Style.ChildBorderSize = backup_border_size;

    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;
    child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;

    // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
    // While this is not really documented/defined, it seems that the expected thing to do.
    if (child_window->BeginCount == 1)
        parent_window->DC.CursorPos = child_window->Pos;

    // Process navigation-in immediately so NavInit can run on first frame
    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
        g.ActiveIdSource = ImGuiInputSource_Nav;
    }
    return ret;
}

bool gui::beginchild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(11, 12));
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
    return beginchildex(str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding);
}

bool gui::beginchild(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    IM_ASSERT(id != 0);
    return beginchildex(NULL, id, size_arg, border, extra_flags);
}

void gui::endchild()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(g.WithinEndChild == false);
    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

    g.WithinEndChild = true;
    if (window->BeginCount > 1)
    {
        End();
    }
    else
    {
        ImVec2 sz = window->Size;
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
            sz.x = ImMax(4.0f, sz.x);
        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
            sz.y = ImMax(4.0f, sz.y);
        End();

        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
        ItemSize(sz);
        if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
        {
            ItemAdd(bb, window->ChildId);
            RenderNavHighlight(bb, window->ChildId);

            // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
            if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
                RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
        }
        else
        {
            // Not navigable into
            ItemAdd(bb, 0);
        }
        if (g.HoveredWindow == window)
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
    }
    PopStyleVar(2);
    g.WithinEndChild = false;
    g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
}

struct checkbox_animation {
    float animation, animation_transparency;
};

bool gui::checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float w = GetWindowWidth() - 24;
    const float square_sz = 17;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect frame_bb(pos + ImVec2(w - square_sz + 2, 0), window->DC.CursorPos + ImVec2(w, square_sz - 3));
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    static std::map <ImGuiID, checkbox_animation> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f, 0.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.animation = ImLerp(it_animation->second.animation, *v ? 1.0f : 0.0f, 0.18f * (1.0f - ImGui::GetIO().DeltaTime));
    it_animation->second.animation_transparency = ImLerp(it_animation->second.animation_transparency, *v ? 1.0f : hovered ? 0.50f : 0.0f, 0.18f * (1.0f - ImGui::GetIO().DeltaTime));

    RenderNavHighlight(total_bb, id);

    window->DrawList->AddCircleFilled((frame_bb.Min + frame_bb.Max) / 2, 4.0f + (3.0f * it_animation->second.animation), ImColor(accent_c[0], accent_c[1], accent_c[2], anim * it_animation->second.animation_transparency), 30);
    RenderCheckMark(window->DrawList, frame_bb.Min + ImVec2(4, 4), ImColor(1.0f, 1.0f, 1.0f, anim * it_animation->second.animation), 4.0f + (3.0f * it_animation->second.animation));

    window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, ImColor(0.0f, 0.0f, 0.0f, anim * 0.35f), 9.0f);

    if (label_size.x > 0.0f)
        window->DrawList->AddText(total_bb.Min, toVec4(70, 70, 70, anim), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

static const char* PatchFormatStringFloatToInt(const char* fmt)
{
    if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
        return "%d";
    const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
    const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
    if (fmt_end > fmt_start && fmt_end[-1] == 'f')
    {
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        if (fmt_start == fmt && fmt_end[0] == 0)
            return "%d";
        const char* tmp_format;
        ImFormatStringToTempBuffer(&tmp_format, NULL, "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
        return tmp_format;
#else
        IM_ASSERT(0 && "DragInt(): Invalid format string!"); // Old versions used a default parameter of "%.0f", please replace with e.g. "%d"
#endif
    }
    return fmt;
}

struct slider_animation {
    float speed, value, animation;
};

bool gui::sliderscalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = GetWindowWidth() - 24;

    static std::map <ImGuiID, slider_animation> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f, 0.0f, 0.7f } });
        it_animation = animation.find(id);
    }

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + 1));
    const ImRect frame_bb(total_bb.Min + ImVec2(window->Size.x - 163 + (36 * it_animation->second.animation), 5), window->DC.CursorPos + ImVec2(window->Size.x - 24, 12));

    const ImRect number_bb(ImVec2(total_bb.Max.x - 140, total_bb.Min.y), ImVec2(total_bb.Max.x - 106, total_bb.Max.y));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
        format = PatchFormatStringFloatToInt(format);

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id);
    const bool number_hovered = ItemHoverable(number_bb, id);
    const bool clicked = (hovered && g.IO.MouseClicked[0]);
    if (clicked)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    if ((frame_bb.Min.x + it_animation->second.value) < grab_bb.Min.x) {
        it_animation->second.speed = (grab_bb.Min.x - (frame_bb.Min.x + it_animation->second.value)) / 12.0f;
        it_animation->second.value += it_animation->second.speed / 60.0f * 140.0f;
    }
    else if ((frame_bb.Min.x + it_animation->second.value) > grab_bb.Min.x) {
        it_animation->second.speed = ((frame_bb.Min.x + it_animation->second.value - grab_bb.Min.x)) / 12.0f;
        it_animation->second.value -= it_animation->second.speed / 60.0f * 140.0f;
    }

    it_animation->second.animation = ImLerp(it_animation->second.animation, (IsItemActive() || hovered && !number_hovered ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(frame_bb.Min + ImVec2(0, 2), frame_bb.Max - ImVec2(0, 2), toVec4(80, 80, 80, anim), 4.0f);
    window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(frame_bb.Min.x + it_animation->second.value + 2, frame_bb.Max.y), ImColor(accent_c[0] - 0.04f, accent_c[1] - 0.04f, accent_c[2] - 0.04f, anim), 4.0f);
    window->DrawList->AddCircleFilled(ImVec2(frame_bb.Min.x - 3 + it_animation->second.value + 3, grab_bb.Min.y + 1), 6.0f, ImColor(accent_c[0], accent_c[1], accent_c[2], anim), 30);

    window->DrawList->AddText(total_bb.Min, toVec4(70, 70, 70, anim), label);

    PushStyleColor(ImGuiCol_Text, ImVec4(toVec4(70, 70, 70, anim * it_animation->second.animation))); {
        RenderTextClipped(number_bb.Min, number_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));
    } PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return value_changed;
}

bool gui::sliderfloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
    return sliderscalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
}

bool gui::sliderint(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return sliderscalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

struct combo_animation {
    float open_anim;
};

bool gui::begin_combo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
    const float size = GetWindowWidth() - 24;

    ImGuiIO& io = ImGui::GetIO();

    const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(size, 16));
    const ImRect clickable(window->DC.CursorPos + ImVec2(window->Size.x - 164, -2), window->DC.CursorPos + ImVec2(size, 18));

    const ImVec2 arrow_size = io.Fonts->Fonts[2]->CalcTextSizeA(7.0f, FLT_MAX, 0.0f, "A");

    ItemSize(rect, style.FramePadding.y);
    if (!ItemAdd(clickable, id, &rect))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(clickable, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    static std::map <ImGuiID, combo_animation> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.open_anim = ImLerp(it_animation->second.open_anim, popup_open ? 1.0f : 0.0f, 0.04f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(clickable.Min, clickable.Max, toVec4(225, 226, 230, anim), 2.0f);
    window->DrawList->AddRect(clickable.Min, clickable.Max, ImColor(0.0f, 0.0f, 0.0f, anim * 0.15f), 2.0f);

    window->DrawList->AddText(rect.Min, toVec4(70, 70, 70, anim), label);

    PushStyleColor(ImGuiCol_Text, ImVec4(toVec4(70, 70, 70, anim))); {
        RenderTextClipped(clickable.Min + ImVec2(5, 2), clickable.Max - ImVec2(19, -47), preview_value, NULL, NULL, ImVec2(0.0f, 0.0f));
    } PopStyleColor();

    window->DrawList->AddText(io.Fonts->Fonts[2], 7.0f, ImVec2(clickable.Max.x - 15, (clickable.Min.y + clickable.Max.y) / 2 - arrow_size.y / 2), toVec4(70, 70, 70, anim), "A");

    if (!popup_open)
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    // Set popup size
    float w = clickable.GetWidth();
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
        int popup_max_height_in_items = -1;
        if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
        SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items) * it_animation->second.open_anim));
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if (ImGuiWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(clickable.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, clickable, ImGuiPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos + ImVec2(0, 3));
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5)); // Horizontally align ourselves with the framed text
    PushStyleVar(ImGuiStyleVar_PopupRounding, 2.0f);
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
    PushStyleColor(ImGuiCol_PopupBg, ImVec4(ImColor(225, 226, 230)));
    PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.15f));
    bool ret = Begin(name, NULL, window_flags | ImGuiWindowFlags_NoScrollbar);
    PopStyleVar(3);
    PopStyleColor(2);
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    return true;
}

void gui::end_combo()
{
    EndPopup();
}

// Old API, prefer using BeginCombo() nowadays if you can.
bool gui::combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!begin_combo(label, preview_value, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        PushID(i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";
        if (selectable(item_text, item_selected))
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

// Getter for the old Combo() API: const char*[]
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}

// Getter for the old Combo() API: "item1\0item2\0item3\0"
static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
    // FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
    const char* items_separated_by_zeros = (const char*)data;
    int items_count = 0;
    const char* p = items_separated_by_zeros;
    while (*p)
    {
        if (idx == items_count)
            break;
        p += strlen(p) + 1;
        items_count++;
    }
    if (!*p)
        return false;
    if (out_text)
        *out_text = p;
    return true;
}

// Combo box helper allowing to pass an array of strings.
bool gui::combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

// Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
bool gui::combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

void gui::multicombo(const char* label, bool combos[], const char* items[], int items_count)
{
    std::vector<std::string> vec;
    static std::string preview;
    for (int i = 0, j = 0; i < items_count; i++)
    {
        if (combos[i])
        {
            vec.push_back(items[i]);
            if (j > 2)
                preview = vec.at(0) + ", " + vec.at(1) + ", " + vec.at(2) + ", ...";
            else if (j)
                preview += ", " + (std::string)items[i];
            else
                preview = items[i];

            j++;
        }
    }
    if (begin_combo(label, preview.c_str()))
    {
        for (int i = 0; i < items_count; i++) {
            selectable(items[i], &combos[i], ImGuiSelectableFlags_DontClosePopups);
        }
        end_combo();
    }

    preview = "None";
}

struct selectable_animation {
    float transparency;
};

bool gui::selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb(min_x, pos.y, text_max.x, text_max.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
        const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    if (span_all_columns)
    {
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    }

    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
    if (span_all_columns)
    {
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }

    if (!item_add)
        return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global) // Only testing this as an optimization
        BeginDisabled();

    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if (span_all_columns && window->DC.CurrentColumns)
        PushColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePushBackgroundChannel();

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
    if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
    if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
    if (flags & ImGuiSelectableFlags_AllowItemOverlap) { button_flags |= ImGuiButtonFlags_AllowItemOverlap; }

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

    static std::map <ImGuiID, selectable_animation> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 1.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.transparency = ImLerp(it_animation->second.transparency, hovered || selected ? 1.0f : 0.6f, 0.09f * (1.0f - ImGui::GetIO().DeltaTime));

    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == window->DC.NavFocusScopeIdCurrent)
        if (g.NavJustMovedToId == id)
            selected = pressed = true;

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
    {
        if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavFocusScopeIdCurrent, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
            g.NavDisableHighlight = true;
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & ImGuiSelectableFlags_AllowItemOverlap)
        SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    if (held && (flags & ImGuiSelectableFlags_DrawHoveredWhenHeld))
        hovered = true;

    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);

    if (span_all_columns && window->DC.CurrentColumns)
        PopColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePopBackgroundChannel();

    PushStyleColor(ImGuiCol_Text, ImVec4(toVec4(70, 70, 70, it_animation->second.transparency))); {
        RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
    } PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed; //-V1020
}

bool gui::selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (selectable(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

const char* const KeyNames[] = {
    "Unknown",
    "LBUTTON",
    "RBUTTON",
    "CANCEL",
    "MBUTTON",
    "XBUTTON1",
    "XBUTTON2",
    "Unknown",
    "BACK",
    "TAB",
    "Unknown",
    "Unknown",
    "CLEAR",
    "RETURN",
    "Unknown",
    "Unknown",
    "SHIFT",
    "CONTROL",
    "MENU",
    "PAUSE",
    "CAPITAL",
    "KANA",
    "Unknown",
    "JUNJA",
    "FINAL",
    "KANJI",
    "Unknown",
    "ESCAPE",
    "CONVERT",
    "NONCONVERT",
    "ACCEPT",
    "MODECHANGE",
    "SPACE",
    "PRIOR",
    "NEXT",
    "END",
    "HOME",
    "LEFT",
    "UP",
    "RIGHT",
    "DOWN",
    "SELECT",
    "PRINT",
    "EXECUTE",
    "SNAPSHOT",
    "INSERT",
    "DELETE",
    "HELP",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "LWIN",
    "RWIN",
    "APPS",
    "Unknown",
    "SLEEP",
    "NUMPAD0",
    "NUMPAD1",
    "NUMPAD2",
    "NUMPAD3",
    "NUMPAD4",
    "NUMPAD5",
    "NUMPAD6",
    "NUMPAD7",
    "NUMPAD8",
    "NUMPAD9",
    "MULTIPLY",
    "ADD",
    "SEPARATOR",
    "SUBTRACT",
    "DECIMAL",
    "DIVIDE",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "NUMLOCK",
    "SCROLL",
    "OEM_NEC_EQUAL",
    "OEM_FJ_MASSHOU",
    "OEM_FJ_TOUROKU",
    "OEM_FJ_LOYA",
    "OEM_FJ_ROYA",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "Unknown",
    "LSHIFT",
    "RSHIFT",
    "LCONTROL",
    "RCONTROL",
    "LMENU",
    "RMENU"
};

#define LBUTTON        0x01
#define RBUTTON        0x02
#define MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */
#define XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#define BACK           0x08
#define RMENU          0xA5

bool gui::hotkey(const char* label, int* k)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiIO& io = g.IO;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float width = 140;
    const float height = 17;

    const ImRect frame_bb(window->DC.CursorPos + ImVec2(window->Size.x - 24 - width, -2), window->DC.CursorPos + ImVec2(window->Size.x - 24, height));
    const ImRect total_bb(window->DC.CursorPos, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

    ItemSize(frame_bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, id))
        return false;

    const bool hovered = IsItemHovered();

    if (hovered)
        SetHoveredID(id);

    const bool user_clicked = hovered && io.MouseClicked[0];

    if (user_clicked) {
        if (g.ActiveId != id) {
            // Start edition
            memset(io.MouseDown, 0, sizeof(io.MouseDown));
            memset(io.KeysDown, 0, sizeof(io.KeysDown));
            *k = 0;
        }
        SetActiveID(id, window);
        FocusWindow(window);
    }
    else if (io.MouseClicked[0]) {
        // Release focus when we click outside
        if (g.ActiveId == id)
            ClearActiveID();
    }

    bool value_changed = false;
    int key = *k;

    if (g.ActiveId == id) {
        for (auto i = 0; i < 5; i++) {
            if (io.MouseDown[i]) {
                switch (i) {
                case 0:
                    key = LBUTTON;
                    break;
                case 1:
                    key = RBUTTON;
                    break;
                case 2:
                    key = MBUTTON;
                    break;
                case 3:
                    key = XBUTTON1;
                    break;
                case 4:
                    key = XBUTTON2;
                    break;
                }
                value_changed = true;
                ClearActiveID();
            }
        }
        if (!value_changed) {
            for (auto i = BACK; i <= RMENU; i++) {
                if (io.KeysDown[i]) {
                    key = i;
                    value_changed = true;
                    ClearActiveID();
                }
            }
        }

        if (IsKeyPressedMap(ImGuiKey_Escape)) {
            *k = 0;
            ClearActiveID();
        }
        else {
            *k = key;
        }
    }

    // Render
    // Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value. We set buf to NULL to prevent accidental usage from now on.
    std::string buf_display = "None";

    RenderFrame(frame_bb.Min, frame_bb.Max, toVec4(225, 226, 230, anim), false, 2.0f);
    window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, ImColor(0.0f, 0.0f, 0.0f, anim * 0.15f), 2.0f);

    if (*k != 0 && g.ActiveId != id) {
        buf_display = KeyNames[*k];
    }
    else if (g.ActiveId == id) {
        buf_display = "...";
    }

    RenderTextClipped(frame_bb.Min, frame_bb.Max, buf_display.c_str(), NULL, NULL, ImVec2(0.5f, 0.5f));

    if (label_size.x > 0) {
        RenderText(total_bb.Min, label);
    }

    return value_changed;
}

struct button_animation {
    float transparency;
};

bool gui::button_ex(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

    static std::map <ImGuiID, selectable_animation> animation;
    auto it_animation = animation.find(id);
    if (it_animation == animation.end()) {
        animation.insert({ id, { 0.0f } });
        it_animation = animation.find(id);
    }

    it_animation->second.transparency = ImLerp(it_animation->second.transparency, hovered ? 0.25f : 0.15f, 0.03f * (1.0f - ImGui::GetIO().DeltaTime));

    // Render
    const ImU32 col = (held && hovered) ? toVec4(205, 206, 210, anim) : hovered ? toVec4(215, 216, 220, anim) : toVec4(225, 226, 230, anim);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, false, 3.0f);
    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(0.0f, 0.0f, 0.0f, anim * it_animation->second.transparency), 3.0f);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool gui::button(const char* label, const ImVec2& size_arg)
{
    return button_ex(label, size_arg, ImGuiButtonFlags_None);
}
#endif