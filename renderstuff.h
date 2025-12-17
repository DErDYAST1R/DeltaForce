#pragma once
bool isExcluded;
float testvalue;

enum class Language {
    English,
    Chinese
};

Language current_language = Language::English;

const char* T(const char* en, const char* zh)
{
    return (current_language == Language::English) ? en : zh;
}

namespace Pointer
{
    uintptr_t UWorld;
    uintptr_t UNetDriver;
    uintptr_t PersistentLevel;
    int ActorCount;
    uintptr_t ActorsArray;
    uintptr_t GameState;
    uintptr_t GameInstance;
    uintptr_t LocalPlayers;
    uintptr_t LocalPlayer;
    uintptr_t PlayerController;
    uintptr_t LocalPawn;
    uintptr_t LocalPlayerState;
    uintptr_t RootComponent;
    Vector3 RelativeLocation;
    uintptr_t PlayerCameraManager;
    uintptr_t LocalMesh;
    FCameraCacheEntry CameraCache;

    float closest_distance;
    uintptr_t closest_mesh;
    uintptr_t closest_entity;
    uintptr_t closest_entityroot_comp;
    uintptr_t closest_bonearray;
}

inline Vector3 GetEntityBone(uintptr_t Mesh, uintptr_t BoneArray, int BoneId);
inline Vector2 ProjectWorldToScreen(FMinimalViewInfo ViewInfo, Vector3 World);

namespace settings
{
    bool awaiting_keybind_1 = false;

    bool prediction;
    bool priceitem;
    bool recoilaim;
    bool shieldESP;
    bool playername;
    bool healthESP;
    float maxitemdist = 100.f;
    bool showbots = true;
    float max_item_value = 0;
    bool itemdist;
    bool itemname;
    bool drawground_item;
    bool drawcontainer_item;
    bool drawdeathbox;
    bool itembox;
    bool drawdistance;
    float render_distance = 250.0f;
    float render_distance_bot = 100.0f;
    bool skeleton;
    bool show_fov;
    bool drawbox = true;
    bool armorstuff;
    bool aimbot;
    float smoothness = 1;
    float fov = 150;
    bool vischeck;
    bool teamcheck_op;
    bool teamcheck_war;

    int hitbox = 0; // Default to the first hitbox
    const char* hitbox_items[] = {
        "Head", "Neck", "Chest"
    };

    const char* hitbox_items_en[] = {
"Head", "Neck", "Chest", "Left Foot", "Right Foot", "Left Lower Arm", "Right Lower Arm"
    };

    const char* hitbox_items_zh[] = {
        u8"头部", u8"脖子", u8"胸部", u8"左脚", u8"右脚", u8"左下臂", u8"右下臂"
    };
}

int width = GetSystemMetrics(SM_CXSCREEN);
int height = GetSystemMetrics(SM_CYSCREEN);
int screen_center_x = width / 2;
int screen_center_y = height / 2;
static int selected_key = VK_RBUTTON; // Default keybind
static bool awaiting_keybind = false; // Flag to check if we're waiting for a keybind
static int speedkey = VK_LSHIFT; // Default keybind
static bool awaitingspeed_keybind = false; // Flag to check if we're waiting for a keybind
HWND hwnd;

// Function to convert VK_ key codes to readable strings
std::string GetKeyNameAB(int vk_code)
{
    char name[128];
    if (GetKeyNameTextA(MapVirtualKeyA(vk_code, MAPVK_VK_TO_VSC) << 16, name, sizeof(name)))
        return std::string(name);
    switch (vk_code) {
    case VK_LBUTTON: return "Mouse Left";
    case VK_RBUTTON: return "Mouse Right";
    case VK_MBUTTON: return "Mouse Middle";
    case VK_XBUTTON1: return "Mouse Button 4";
    case VK_XBUTTON2: return "Mouse Button 5";
    case VK_SHIFT: return "Shift";
    case VK_CONTROL: return "Ctrl";
    case VK_MENU: return "Alt";
    case VK_SPACE: return "Space";
    case VK_TAB: return "Tab";
    case VK_CAPITAL: return "Caps Lock";
    default:
        return "Unknown Key";
    }
}
static int current_tab = 0;
inline void TextCentered(const char* text)
{
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f);
    ImGui::TextUnformatted(text);
}

#ifdef LUKE2
void menu_red()
{
    // --- Style & Colors ---
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarSize = 10.0f;
    style.ItemSpacing = ImVec2(8, 6);
    style.FrameBorderSize = 1.0f;
    style.WindowPadding = ImVec2(12, 12);

    ImVec4 colBg(0.10f, 0.10f, 0.10f, 1.0f);
    ImVec4 colFrame(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 colRed(0.90f, 0.05f, 0.05f, 1.0f);
    ImVec4 colRedHov(1.0f, 0.25f, 0.25f, 1.0f);
    ImVec4 colRedAct(0.75f, 0.05f, 0.05f, 1.0f);
    ImVec4 colText(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 colTextDark(0.1f, 0.1f, 0.1f, 1.0f);

    style.Colors[ImGuiCol_WindowBg] = colBg;
    style.Colors[ImGuiCol_FrameBg] = colFrame;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = colRed;
    style.Colors[ImGuiCol_PopupBg] = colFrame;
    style.Colors[ImGuiCol_Button] = colRed;
    style.Colors[ImGuiCol_ButtonHovered] = colRedHov;
    style.Colors[ImGuiCol_ButtonActive] = colRedAct;
    style.Colors[ImGuiCol_Header] = colRed;
    style.Colors[ImGuiCol_HeaderHovered] = colRedHov;
    style.Colors[ImGuiCol_HeaderActive] = colRedAct;
    style.Colors[ImGuiCol_CheckMark] = colRed;
    style.Colors[ImGuiCol_SliderGrab] = colRed;
    style.Colors[ImGuiCol_SliderGrabActive] = colRedHov;
    style.Colors[ImGuiCol_Text] = colText;

    static int messageTimer = 0; // นับเฟรม
    static ImVec4 messageColor = ImVec4(0, 1, 0, 1); // สีข้อความ
    static std::string messageText = "";

    // --- Smaller Window Size ---
    ImGui::SetNextWindowSize(ImVec2(570, 425), ImGuiCond_Always);
    ImGui::Begin("Delta Force", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // — Title & Underline —
    const char* txt1 = T("Delta", u8"三角洲");
    const char* txt2 = T("Force", u8"力量");
    ImVec2 s1 = ImGui::CalcTextSize(txt1);
    ImVec2 s2 = ImGui::CalcTextSize(txt2);
    float totalW = s1.x + s2.x;
    ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - totalW) * 0.5f, s2.y / 2));
    ImGui::TextColored(colText, txt1); ImGui::SameLine(0, 0);
    ImGui::TextColored(colRed, txt2);

    // — Sidebar Tabs —
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, 120); // Smaller sidebar

    static int tab = 0;
    const char* tabs[] = { T("ESP", u8"透视"), T("AIM", u8"自瞄"), T("Misc", u8"杂项") };
    for (int i = 0; i < 3; ++i) {
        bool sel = (tab == i);
        ImGui::PushStyleColor(ImGuiCol_Button, sel ? colRed : colFrame);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colRedHov);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, colRedAct);
        ImGui::PushStyleColor(ImGuiCol_Text, sel ? colTextDark : colText);
        if (ImGui::Button(tabs[i], ImVec2(-FLT_MIN, 36))) // Slightly smaller height
            tab = i;
        ImGui::PopStyleColor(4);
    }

    // — Language Toggle —
    ImVec2 winSz = ImGui::GetWindowSize();
    ImGui::SetCursorPosY(winSz.y - 32);
    if (ImGui::Button(current_language == Language::English ? "EN" : u8"中文", ImVec2(48, 20)))
        current_language = current_language == Language::English ? Language::Chinese : Language::English;

    ImGui::NextColumn();

    // — Main Content —
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    if (tab == 0) // ESP Tab
    {
        ImGui::Columns(2, "esp_cols", false);
        ImGui::Checkbox(T("Draw Box", u8"绘制方框"), &settings::drawbox);
        ImGui::Checkbox(T("Draw Skeleton", u8"绘制骨架"), &settings::skeleton);
        ImGui::Checkbox(T("Draw Health", u8"绘制生命值"), &settings::healthESP);
        ImGui::Checkbox(T("Draw Name", u8"绘制名字"), &settings::playername);
        ImGui::Checkbox(T("Draw Distance", u8"绘制距离"), &settings::drawdistance);
        ImGui::NextColumn();
        ImGui::Checkbox(T("Draw Armor", u8"绘制护甲"), &settings::shieldESP);
        ImGui::Checkbox(T("Draw Bots", u8"绘制机器人"), &settings::showbots);
        ImGui::Checkbox(T("Visible Check", u8"可见性检查"), &settings::vischeck);
        ImGui::Checkbox(T("Team Check War", u8"队伍检查 (战争)"), &settings::teamcheck_war);
        ImGui::Checkbox(T("Team Check Op", u8"队伍检查 (行动)"), &settings::teamcheck_op);
        ImGui::Columns(1);

        ImGui::Separator();

        // — Sliders with hidden labels for clean layout —
        PushItemWidth(-FLT_MIN);
        ImGui::Text(T("Render Distance", u8"渲染距离"));
        ImGui::SliderFloat("##render_dist", &settings::render_distance, 1, 500, "%.0f");
        ImGui::Text(T("Render Distance BOT", u8"渲染距离 (机器人)"));
        ImGui::SliderFloat("##render_dist_bot", &settings::render_distance_bot, 1, 500, "%.0f");
        PopItemWidth();

        ImGui::Separator();
        ImGui::Columns(2, "item_cols", false);
        ImGui::Checkbox(T("Item Price", u8"物品价格"), &settings::priceitem);
        ImGui::Checkbox(T("Item Death Box", u8"死亡物品箱"), &settings::drawdeathbox);
        ImGui::NextColumn();
        ImGui::Checkbox(T("Show Ground Items", u8"显示地面物品"), &settings::drawground_item);
        ImGui::Columns(1);

        ImGui::Separator();
        PushItemWidth(-FLT_MIN);
        ImGui::Text(T("Item Max Distance", u8"物品最大距离"));
        ImGui::SliderFloat("##item_max_dist", &settings::maxitemdist, 1, 500, "%.0f");
        ImGui::Text(T("Item Min Value", u8"物品最低价值"));
        ImGui::SliderFloat("##item_min_val", &settings::max_item_value, 0, 2000000, "%.0f");
        PopItemWidth();
    }
    else if (tab == 1) // AIM Tab
    {
        ImGui::Columns(2, "aim_cols", false);
        ImGui::Checkbox(T("Enable Aimbot", u8"启用自瞄"), &settings::aimbot);
        ImGui::Checkbox(T("Prediction", u8"预测弹道"), &settings::prediction);
        ImGui::NextColumn();
        ImGui::Checkbox(T("Show Fov Circle", u8"显示Fov圆圈"), &settings::show_fov);
        //ImGui::Checkbox(T("No Recoil", u8"无后座力"), &settings::recoilaim);
        ImGui::Columns(1);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::PushItemWidth(-FLT_MIN);

        ImGui::Text(T("Smoothness", u8"平滑度"));
        ImGui::SliderFloat("##smoothness_slider", &settings::smoothness, 1.0f, 50.0f, "%.0f");

        ImGui::Text(T("Fov", u8"视野角度"));
        ImGui::SliderFloat("##fov_slider", &settings::fov, 1.0f, 250.0f, "%.0f");

        ImGui::Text(T("Hitbox", u8"命中部位"));
        ImGui::Combo("##hitbox_combo", &settings::hitbox,
            current_language == Language::Chinese ? settings::hitbox_items_zh : settings::hitbox_items_en,
            IM_ARRAYSIZE(settings::hitbox_items_en));

        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "key_cols", false);

        // Key 1
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s 1: %s", T("Aimbot Key", u8"自瞄按键"), GetKeyNameAB(selected_key).c_str());

        if (settings::awaiting_keybind_1) {
            ImGui::Text(T("Press any key...", u8"按下任意键..."));
            for (int k = 1; k < 256; ++k) {
                if (GetAsyncKeyState(k) & 0x8000) {
                    selected_key = k;
                    settings::awaiting_keybind_1 = false;
                }
            }
        }
        else if (ImGui::Button(T("Set Key 1", u8"设置按键1"))) {
            settings::awaiting_keybind_1 = true;
        }

        ImGui::Columns(1);
    }
    else // MISC Tab
    {
        TextCentered(T("Use this tab to save/load your settings or reset to defaults.", u8"使用此标签页保存/加载设置或重置为默认。"));
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::BeginGroup();

        float buttonWidth = 120.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = buttonWidth * 2 + spacing;
        float availWidth = ImGui::GetContentRegionAvail().x;
        float offsetX = (availWidth - totalWidth) * 0.5f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);



        ImGui::EndGroup();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();


        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "aim_cols", false);
        ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::End();
}

void menu_purpleblue()
{
    // --- Style & Colors ---
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarSize = 10.0f;
    style.ItemSpacing = ImVec2(8, 6);
    style.FrameBorderSize = 1.0f;
    style.WindowPadding = ImVec2(12, 12);

    // --- Color Palette ---
    ImVec4 colBg(0.08f, 0.09f, 0.16f, 1.0f);          // Deep navy background
    ImVec4 colFrame(0.14f, 0.15f, 0.25f, 1.0f);       // Muted frame
    ImVec4 colAccent(0.45f, 0.25f, 0.85f, 1.0f);      // Main purple-blue accent
    ImVec4 colAccentHov(0.60f, 0.40f, 0.95f, 1.0f);   // Hovered accent
    ImVec4 colAccentAct(0.35f, 0.15f, 0.75f, 1.0f);   // Active accent
    ImVec4 colText(0.95f, 0.96f, 1.0f, 1.0f);         // Soft white text
    ImVec4 colTextDark(0.08f, 0.08f, 0.12f, 1.0f);    // For contrast text

    // --- Apply Colors ---
    style.Colors[ImGuiCol_WindowBg] = colBg;
    style.Colors[ImGuiCol_FrameBg] = colFrame;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.26f, 0.36f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = colAccent;
    style.Colors[ImGuiCol_PopupBg] = colFrame;
    style.Colors[ImGuiCol_Button] = colAccent;
    style.Colors[ImGuiCol_ButtonHovered] = colAccentHov;
    style.Colors[ImGuiCol_ButtonActive] = colAccentAct;
    style.Colors[ImGuiCol_Header] = colAccent;
    style.Colors[ImGuiCol_HeaderHovered] = colAccentHov;
    style.Colors[ImGuiCol_HeaderActive] = colAccentAct;
    style.Colors[ImGuiCol_CheckMark] = colAccentHov;
    style.Colors[ImGuiCol_SliderGrab] = colAccent;
    style.Colors[ImGuiCol_SliderGrabActive] = colAccentHov;
    style.Colors[ImGuiCol_Text] = colText;

    static int messageTimer = 0; // นับเฟรม
    static ImVec4 messageColor = ImVec4(0, 1, 0, 1); // สีข้อความ
    static std::string messageText = "";

    // --- Smaller Window Size ---
    ImGui::SetNextWindowSize(ImVec2(570, 425), ImGuiCond_Always);
    ImGui::Begin("Delta Elite", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // — Title & Underline —
    const char* txt1 = T("Delta", u8"三角洲");
    const char* txt2 = T("Elite", u8"力量");
    ImVec2 s1 = ImGui::CalcTextSize(txt1);
    ImVec2 s2 = ImGui::CalcTextSize(txt2);
    float totalW = s1.x + s2.x;
    ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - totalW) * 0.5f, s2.y / 2));
    ImGui::TextColored(colText, txt1); ImGui::SameLine(0, 0);
    ImGui::TextColored(colAccent, txt2);

    // — Sidebar Tabs —
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, 120); // Smaller sidebar

    static int tab = 0;
    const char* tabs[] = { T("ESP", u8"透视"), T("AIM", u8"自瞄"), T("Misc", u8"杂项") };
    for (int i = 0; i < 3; ++i) {
        bool sel = (tab == i);
        ImGui::PushStyleColor(ImGuiCol_Button, sel ? colAccent : colFrame);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colAccentHov);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, colAccent);
        ImGui::PushStyleColor(ImGuiCol_Text, sel ? colTextDark : colText);
        if (ImGui::Button(tabs[i], ImVec2(-FLT_MIN, 36))) // Slightly smaller height
            tab = i;
        ImGui::PopStyleColor(4);
    }

    // — Language Toggle —
    ImVec2 winSz = ImGui::GetWindowSize();
    ImGui::SetCursorPosY(winSz.y - 32);
    if (ImGui::Button(current_language == Language::English ? "EN" : u8"中文", ImVec2(48, 20)))
        current_language = current_language == Language::English ? Language::Chinese : Language::English;

    ImGui::NextColumn();

    // — Main Content —
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    if (tab == 0) // ESP Tab
    {
        ImGui::Columns(2, "esp_cols", false);
        ImGui::Checkbox(T("Draw Box", u8"绘制方框"), &settings::drawbox);
        ImGui::Checkbox(T("Draw Skeleton", u8"绘制骨架"), &settings::skeleton);
        ImGui::Checkbox(T("Draw Health", u8"绘制生命值"), &settings::healthESP);
        ImGui::Checkbox(T("Draw Name", u8"绘制名字"), &settings::playername);
        ImGui::Checkbox(T("Draw Distance", u8"绘制距离"), &settings::drawdistance);
        ImGui::NextColumn();
        ImGui::Checkbox(T("Draw Armor", u8"绘制护甲"), &settings::shieldESP);
        ImGui::Checkbox(T("Draw Bots", u8"绘制机器人"), &settings::showbots);
        ImGui::Checkbox(T("Visible Check", u8"可见性检查"), &settings::vischeck);
        ImGui::Checkbox(T("Team Check War", u8"队伍检查 (战争)"), &settings::teamcheck_war);
        ImGui::Checkbox(T("Team Check Op", u8"队伍检查 (行动)"), &settings::teamcheck_op);
        ImGui::Columns(1);

        ImGui::Separator();

        // — Sliders with hidden labels for clean layout —
        PushItemWidth(-FLT_MIN);
        ImGui::Text(T("Render Distance", u8"渲染距离"));
        ImGui::SliderFloat("##render_dist", &settings::render_distance, 1, 500, "%.0f");
        ImGui::Text(T("Render Distance BOT", u8"渲染距离 (机器人)"));
        ImGui::SliderFloat("##render_dist_bot", &settings::render_distance_bot, 1, 500, "%.0f");
        PopItemWidth();

        ImGui::Separator();
        ImGui::Columns(2, "item_cols", false);
        ImGui::Checkbox(T("Item Price", u8"物品价格"), &settings::priceitem);
        ImGui::Checkbox(T("Item Death Box", u8"死亡物品箱"), &settings::drawdeathbox);
        ImGui::NextColumn();
        ImGui::Checkbox(T("Show Ground Items", u8"显示地面物品"), &settings::drawground_item);
        ImGui::Columns(1);

        ImGui::Separator();
        PushItemWidth(-FLT_MIN);
        ImGui::Text(T("Item Max Distance", u8"物品最大距离"));
        ImGui::SliderFloat("##item_max_dist", &settings::maxitemdist, 1, 500, "%.0f");
        ImGui::Text(T("Item Min Value", u8"物品最低价值"));
        ImGui::SliderFloat("##item_min_val", &settings::max_item_value, 0, 2000000, "%.0f");
        PopItemWidth();
    }
    else if (tab == 1) // AIM Tab
    {
        ImGui::Columns(2, "aim_cols", false);
        ImGui::Checkbox(T("Enable Aimbot", u8"启用自瞄"), &settings::aimbot);
        ImGui::Checkbox(T("Prediction", u8"预测弹道"), &settings::prediction);
        ImGui::NextColumn();
        ImGui::Checkbox(T("Show Fov Circle", u8"显示Fov圆圈"), &settings::show_fov);
        //ImGui::Checkbox(T("No Recoil", u8"无后座力"), &settings::recoilaim);
        ImGui::Columns(1);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::PushItemWidth(-FLT_MIN);

        ImGui::Text(T("Smoothness", u8"平滑度"));
        ImGui::SliderFloat("##smoothness_slider", &settings::smoothness, 1.0f, 50.0f, "%.0f");

        ImGui::Text(T("Fov", u8"视野角度"));
        ImGui::SliderFloat("##fov_slider", &settings::fov, 1.0f, 250.0f, "%.0f");

        ImGui::Text(T("Hitbox", u8"命中部位"));
        ImGui::Combo("##hitbox_combo", &settings::hitbox,
            current_language == Language::Chinese ? settings::hitbox_items_zh : settings::hitbox_items_en,
            IM_ARRAYSIZE(settings::hitbox_items_en));

        ImGui::PopItemWidth();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "key_cols", false);

        // Key 1
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s 1: %s", T("Aimbot Key", u8"自瞄按键"), GetKeyNameAB(selected_key).c_str());

        if (settings::awaiting_keybind_1) {
            ImGui::Text(T("Press any key...", u8"按下任意键..."));
            for (int k = 1; k < 256; ++k) {
                if (GetAsyncKeyState(k) & 0x8000) {
                    selected_key = k;
                    settings::awaiting_keybind_1 = false;
                }
            }
        }
        else if (ImGui::Button(T("Set Key 1", u8"设置按键1"))) {
            settings::awaiting_keybind_1 = true;
        }

        ImGui::Columns(1);
    }
    else // MISC Tab
    {
        TextCentered(T("Use this tab to save/load your settings or reset to defaults.", u8"使用此标签页保存/加载设置或重置为默认。"));
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::BeginGroup();

        float buttonWidth = 120.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float totalWidth = buttonWidth * 2 + spacing;
        float availWidth = ImGui::GetContentRegionAvail().x;
        float offsetX = (availWidth - totalWidth) * 0.5f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);



        ImGui::EndGroup();

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();


        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Columns(2, "aim_cols", false);
        ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Spacing();
    }
    ImGui::EndChild();
    ImGui::End();
}

void menu4()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Buttons
    ImVec4 buttonColor1 = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    ImVec4 buttonHovered1 = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    ImVec4 buttonActive1 = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

    // --- Scrollbar Style ---
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
    style.ScrollbarSize = 11.0f;

    // Desired fixed window size
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);

    // --- Main Color Scheme ---
    ImVec4 headerColor = ImVec4(0.20f, 0.45f, 0.70f, 1.0f); // Vibrant blue header (drawn in background)
    ImVec4 mainBgColor = ImVec4(0.12f, 0.12f, 0.12f, 1.0f); // Dark gray background
    ImVec4 buttonColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f); // Button normal
    ImVec4 buttonHovered = ImVec4(0.35f, 0.35f, 0.35f, 1.0f); // Button hovered
    ImVec4 buttonActive = ImVec4(0.45f, 0.45f, 0.45f, 1.0f); // Button active
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);     // White text
    ImVec4 headingColor = ImVec4(0.0f, 0.85f, 0.85f, 1.0f);   // Bright teal for headings

    // --- Custom Slider & Checkbox Colors ---
    // Adjust these if you prefer different hues.
    ImVec4 customSliderGrab = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
    ImVec4 customSliderGrabActive = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
    ImVec4 customCheckboxBg = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 customCheckboxHovered = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 customCheckboxActive = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 customCheckMarkColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Apply the main window background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, mainBgColor);

    // Create the window without a title bar, collapse, or resize
    if (ImGui::Begin("baruiname", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float headerHeight = 50.0f;
        float sidebarWidth = 150.0f;

        // --- Draw "Header" Title (Centered) ---
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // We only add text (no filled rect) for the header, 
        // but you can fill a rectangle with 'headerColor' if desired.
        const char* title = "Delta Force";
        ImVec2 titleSize = ImGui::CalcTextSize(title);
        ImVec2 titlePos(
            windowPos.x + (windowSize.x - titleSize.x) * 0.5f,
            windowPos.y + (headerHeight - titleSize.y) * 0.5f
        );
        drawList->AddText(titlePos, IM_COL32(255, 255, 255, 255), title);

        // Vertical spacing below the "header"
        ImGui::SetCursorPosY(headerHeight + 10);

        // --- Sidebar (Left Pane) ---
        ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, windowSize.y - headerHeight - 20), true);

        // Push button + text colors for the tab buttons
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActive);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor); // White text on buttons

        if (ImGui::Button("Visuals", ImVec2(sidebarWidth - 20, 40)))
            current_tab = 0;
#ifdef ESPONLY
#else
        if (ImGui::Button("Aimbot", ImVec2(sidebarWidth - 20, 40)))
            current_tab = 1;
        if (ImGui::Button("Misc", ImVec2(sidebarWidth - 20, 40)))
            current_tab = 2;
#endif
        ImGui::PopStyleColor(4);
        ImGui::EndChild();

        ImGui::SameLine();

        // --- Main Content Area (Right Pane) ---
        ImGui::BeginChild("MainContent", ImVec2(windowSize.x - sidebarWidth - 20, windowSize.y - headerHeight - 20), true);

        // Tab content:
        if (current_tab == 0)  // Visuals Tab
        {
            ImGui::TextColored(headingColor, "VISUAL SETTINGS");
            ImGui::Separator();

            // Push custom style colors for text, sliders, and checkboxes
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::Checkbox("Draw Box", &settings::drawbox);
            ImGui::Checkbox("Draw Skeleton", &settings::skeleton);
            ImGui::Checkbox("Draw Health", &settings::healthESP);
            ImGui::Checkbox("Draw Name", &settings::playername);
            ImGui::Checkbox("Draw Distance", &settings::drawdistance);
            ImGui::Checkbox("Draw Armor", &settings::shieldESP);
            ImGui::Checkbox("Draw Bots", &settings::showbots);

            ImGui::Checkbox("Visible Check", &settings::vischeck);
            ImGui::Checkbox("Team Check War", &settings::teamcheck_war);
            ImGui::Checkbox("Team Check Op", &settings::teamcheck_op);
            ImGui::SliderFloat("Render Distance", &settings::render_distance, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Render Distance BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

            ImGui::Checkbox("Show ground items", &settings::drawground_item);
            ImGui::Checkbox("Item Price", &settings::priceitem);
            ImGui::Checkbox("Show DeathBox", &settings::drawdeathbox);
            ImGui::SliderFloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);

            // Pop the 7 style colors we pushed
            ImGui::PopStyleColor(7);
        }
        else if (current_tab == 1)  // Aimbot Tab
        {
            ImGui::TextColored(headingColor, "AIMBOT SETTINGS");
            ImGui::Separator();

            // Push style colors for text, sliders, and checkboxes
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::Checkbox("Enable Aimbot", &settings::aimbot);
            ImGui::Checkbox("Prediction", &settings::prediction);

            ImGui::Checkbox("Show FOV Circle", &settings::show_fov);
            ImGui::SliderFloat("Smoothness", &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("FOV", &settings::fov, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

            // Display the current keybind
            ImGui::Text("Aimbot Key: %s", GetKeyNameAB(selected_key).c_str());

            // Button to set a new keybind
            if (awaiting_keybind)
            {
                ImGui::Text("Press any key...");
                for (int key = 1; key < 256; ++key)
                {
                    if (GetAsyncKeyState(key) & 0x8000)
                    {
                        selected_key = key;
                        awaiting_keybind = false;
                        break;
                    }
                }
            }
            else
            {
                if (ImGui::Button("Set Keybind"))
                {
                    awaiting_keybind = true;
                }
            }
            ImGui::Combo("Hitbox", &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));

            ImGui::PopStyleColor(7);
        }
        else if (current_tab == 2)  // Misc Tab
        {
            ImGui::TextColored(headingColor, "MISC SETTINGS");
            ImGui::Separator();

            // Push style colors for text, sliders, and checkboxes
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);


            ImGui::PopStyleColor(7);
        }

        ImGui::EndChild();  // End MainContent
    }
    ImGui::End();

    // Pop the window background color
    ImGui::PopStyleColor();
}
#elif defined(d4ftBrand)
void menu_red()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // --- Style Tweaks ---
    style.WindowRounding = 4.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarSize = 12.0f;
    style.ItemSpacing = ImVec2(10, 8);
    style.FrameBorderSize = 1.0f;

    // --- Color Scheme: Red ---
    ImVec4 red = ImVec4(0.85f, 0.1f, 0.1f, 1.0f);  // Bright red
    ImVec4 redHover = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);  // Softer hover red
    ImVec4 redActive = ImVec4(0.75f, 0.05f, 0.05f, 1.0f);
    ImVec4 bgDark = ImVec4(0.11f, 0.11f, 0.11f, 1.0f);
    ImVec4 bgFrame = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 blackText = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    // --- Color Overrides ---
    style.Colors[ImGuiCol_WindowBg] = bgDark;
    style.Colors[ImGuiCol_Button] = red;
    style.Colors[ImGuiCol_ButtonHovered] = redHover;
    style.Colors[ImGuiCol_ButtonActive] = redActive;
    style.Colors[ImGuiCol_Text] = white;
    style.Colors[ImGuiCol_Header] = red;
    style.Colors[ImGuiCol_HeaderHovered] = redHover;
    style.Colors[ImGuiCol_HeaderActive] = redActive;
    style.Colors[ImGuiCol_FrameBg] = bgFrame;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = red;
    style.Colors[ImGuiCol_CheckMark] = red;
    style.Colors[ImGuiCol_SliderGrab] = red;
    style.Colors[ImGuiCol_SliderGrabActive] = redHover;
    style.Colors[ImGuiCol_PopupBg] = bgFrame;

    // --- Window ---
    ImGui::SetNextWindowSize(ImVec2(720, 480), ImGuiCond_Always);
    ImGui::Begin("ABUSE", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    // --- Header ---
    const char* text1 = "A";
    const char* text2 = "BUSE";
    ImVec2 textSize1 = ImGui::CalcTextSize(text1);
    ImVec2 textSize2 = ImGui::CalcTextSize(text2);
    float totalTextWidth = textSize1.x + textSize2.x;
    float startX = (ImGui::GetWindowSize().x - totalTextWidth) * 0.5f;

    ImGui::SetCursorPosX(startX);
    ImGui::TextColored(white, text1);
    ImGui::SameLine(0.0f, 0.0f);
    ImGui::TextColored(red, text2);
    ImGui::Separator();

    // --- Tabs Layout ---
    static int tab = 0;
    const char* tabs[] = { "ESP", "AIM", "Misc" };
    ImGui::Columns(2, nullptr, false);
    ImGui::SetColumnWidth(0, 150);

    for (int i = 0; i < IM_ARRAYSIZE(tabs); ++i)
    {
        if (tab == i)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, red);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redHover);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, redActive);
            ImGui::PushStyleColor(ImGuiCol_Text, blackText);
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, bgFrame);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redHover);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, redActive);
            ImGui::PushStyleColor(ImGuiCol_Text, white);
        }

        if (ImGui::Button(tabs[i], ImVec2(-FLT_MIN, 38)))
            tab = i;

        ImGui::PopStyleColor(4);
    }

    ImGui::NextColumn();
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);

    // --- Tab Content ---
    if (tab == 0) // ESP
    {
        ImGui::Checkbox("Draw Box", &settings::drawbox);
        ImGui::Checkbox("Draw Skeleton", &settings::skeleton);
        ImGui::Checkbox("Draw Health", &settings::healthESP);
        ImGui::Checkbox("Draw Name", &settings::playername);
        ImGui::Checkbox("Draw Distance", &settings::drawdistance);
        ImGui::Checkbox("Draw Armor", &settings::shieldESP);
        ImGui::Checkbox("Draw Bots", &settings::showbots);

        ImGui::Checkbox("Visible Check", &settings::vischeck);
        ImGui::Checkbox("Team Check War", &settings::teamcheck_war);
        ImGui::Checkbox("Team Check Op", &settings::teamcheck_op);
        ImGui::SliderFloat("Render Distance", &settings::render_distance, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Render Distance BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

        ImGui::Checkbox("Show ground items", &settings::drawground_item);
        ImGui::Checkbox("Item Price", &settings::priceitem);
        ImGui::Checkbox("Show DeathBox", &settings::drawdeathbox);
        ImGui::SliderFloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);

    }
    else if (tab == 1) // Combat
    {
        ImGui::Checkbox("Enable Aimbot", &settings::aimbot);
        ImGui::Checkbox("Prediction", &settings::prediction);

        ImGui::Checkbox("Show FOV Circle", &settings::show_fov);
        ImGui::SliderFloat("Smoothness", &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
        ImGui::SliderFloat("FOV", &settings::fov, 50, 300, "%.f", ImGuiSliderFlags_NoInput);

        // Display the current keybind
        ImGui::Text("Aimbot Key: %s", GetKeyNameAB(selected_key).c_str());

        // Button to set a new keybind
        if (awaiting_keybind)
        {
            ImGui::Text("Press any key...");
            for (int key = 1; key < 256; ++key)
            {
                if (GetAsyncKeyState(key) & 0x8000)
                {
                    selected_key = key;
                    awaiting_keybind = false;
                    break;
                }
            }
        }
        else
        {
            if (ImGui::Button("Set Keybind"))
            {
                awaiting_keybind = true;
            }
        }
        ImGui::Combo("Hitbox", &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));



    }
    else if (tab == 2) // Utility
    {

    }

    ImGui::EndChild();
    ImGui::End();
}
#elif defined(PANDA)
void menu5()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // --- Color Theme (Reddish) ---
    ImVec4 mainBgColor = ImVec4(0.15f, 0.05f, 0.05f, 1.0f); // Deep dark red
    ImVec4 headerColor = ImVec4(0.6f, 0.1f, 0.1f, 1.0f); // Dark red header
    ImVec4 textColor = ImVec4(1.0f, 0.8f, 0.8f, 1.0f); // Soft pinkish white
    ImVec4 buttonColor = ImVec4(0.4f, 0.1f, 0.1f, 1.0f); // Muted red
    ImVec4 buttonHovered = ImVec4(0.5f, 0.15f, 0.15f, 1.0f);
    ImVec4 buttonActive = ImVec4(0.6f, 0.2f, 0.2f, 1.0f);
    // Custom Checkbox Colors (Reddish Theme)
    ImVec4 customCheckboxBg = ImVec4(0.5f, 0.1f, 0.1f, 1.0f);      // Dark red background
    ImVec4 customCheckboxHovered = ImVec4(0.7f, 0.2f, 0.2f, 1.0f); // Brighter red when hovered
    ImVec4 customCheckboxActive = ImVec4(0.9f, 0.3f, 0.3f, 1.0f);  // Even brighter red when active
    ImVec4 customCheckMarkColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // White checkmark
    ImVec4 textColor2 = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Pure white
    ImVec4 customSliderGrab = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
    ImVec4 customSliderGrabActive = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
    // --- Scrollbar Style ---
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
    style.ScrollbarSize = 11.0f;

    // Apply window background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, mainBgColor);

    ImGui::SetNextWindowSize(ImVec2(650, 450), ImGuiCond_Always);

    if (ImGui::Begin("uihbarname", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
    {
        ImVec2 windowSize = ImGui::GetWindowSize();
        float headerHeight = 28.0f;

        // --- Header Section ---
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 headerStart = ImGui::GetCursorScreenPos();
        drawList->AddRectFilled(headerStart, ImVec2(headerStart.x + windowSize.x, headerStart.y + headerHeight), ImColor(headerColor));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
        ImGui::SetCursorPosX((windowSize.x / 2) - 50);
        ImGui::TextColored(textColor, "SilentCore External");
        ImGui::SetCursorPosY(headerHeight + 10);

        // --- Sidebar Navigation ---
        float sidebarWidth = 180.0f;
        ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, windowSize.y - headerHeight - 20), true);
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActive);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        if (ImGui::Button("Visuals", ImVec2(sidebarWidth - 20, 45)))
            current_tab = 0;
        if (ImGui::Button("Aimbot", ImVec2(sidebarWidth - 20, 45)))
            current_tab = 1;
        if (ImGui::Button("Misc", ImVec2(sidebarWidth - 20, 45)))
            current_tab = 2;

        ImGui::PopStyleColor(4);
        ImGui::EndChild();

        ImGui::SameLine();

        // --- Main Content ---
        ImGui::BeginChild("MainContent", ImVec2(windowSize.x - sidebarWidth - 30, windowSize.y - headerHeight - 20), true);
        if (current_tab == 0) {
            ImGui::TextColored(textColor, "Visual Settings");
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_Text, textColor2);             // White text
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::Checkbox("Draw Box", &settings::drawbox);
            ImGui::Checkbox("Draw Skeleton", &settings::skeleton);
            ImGui::Checkbox("Draw Health", &settings::healthESP);
            ImGui::Checkbox("Draw Name", &settings::playername);
            ImGui::Checkbox("Draw Distance", &settings::drawdistance);
            ImGui::Checkbox("Draw Armor", &settings::shieldESP);
            ImGui::Checkbox("Draw Bots", &settings::showbots);

            ImGui::Checkbox("Visible Check", &settings::vischeck);
            ImGui::Checkbox("Team Check War", &settings::teamcheck_war);
            ImGui::Checkbox("Team Check Op", &settings::teamcheck_op);
            ImGui::SliderFloat("Render Distance", &settings::render_distance, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Render Distance BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

            ImGui::Checkbox("Show ground items", &settings::drawground_item);
            ImGui::Checkbox("Item Price", &settings::priceitem);
            ImGui::Checkbox("Show DeathBox", &settings::drawdeathbox);
            ImGui::SliderFloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);

            ImGui::PopStyleColor(7); // Pop the colors
        }
        else if (current_tab == 1) {
            ImGui::TextColored(textColor, "Aimbot Settings");
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_Text, textColor2);             // White text
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::Checkbox("Enable Aimbot", &settings::aimbot);
            ImGui::Checkbox("Prediction", &settings::prediction);

            ImGui::Checkbox("Show FOV Circle", &settings::show_fov);
            ImGui::SliderFloat("Smoothness", &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("FOV", &settings::fov, 50, 300, "%.f", ImGuiSliderFlags_NoInput);

            // Display the current keybind
            ImGui::Text("Aimbot Key: %s", GetKeyNameAB(selected_key).c_str());

            // Button to set a new keybind
            if (awaiting_keybind)
            {
                ImGui::Text("Press any key...");
                for (int key = 1; key < 256; ++key)
                {
                    if (GetAsyncKeyState(key) & 0x8000)
                    {
                        selected_key = key;
                        awaiting_keybind = false;
                        break;
                    }
                }
            }
            else
            {
                if (ImGui::Button("Set Keybind"))
                {
                    awaiting_keybind = true;
                }
            }
            ImGui::Combo("Hitbox", &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));

            ImGui::PopStyleColor(7); // Pop the colors
        }
        else if (current_tab == 2) {
            ImGui::TextColored(textColor, "Misc Settings");
            ImGui::Separator();
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_Text, textColor2);             // White text
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::PopStyleColor(7); // Pop the colors
        }
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleColor();
}

#elif defined(NotShxdowBrand)
void menu4()
{
    ImGui::Begin("uihname", 0, ImGuiWindowFlags_NoDecoration);
    {
        ImGui::SetWindowSize(ui::size);

        ImGui::SetCursorPos({ 15, 15 });
        ImGui::BeginChild("nav", { ui::nav_width, ImGui::GetWindowHeight() - 30 }, 0, ImGuiWindowFlags_NoBackground);
        {
            ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_ChildBg), ImGui::GetStyle().ChildRounding);
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildRounding, 0, ImGui::GetStyle().ChildBorderSize);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8, 10 });
            ImGui::SetCursorPosY(25);
            ImGui::BeginGroup();
            {
                for (int i = 0; i < ui::tabs.size(); ++i)
                {
                    ui::tab(i);
                    if (i != ui::tabs.size() - 1)
                        ui::separator(20);
                }
            }
            ImGui::EndGroup();
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();

        ImGui::SetCursorPos({ 15 + ui::nav_width - 18 / 2, 25 });
        ImGui::BeginChild("nav_btn_wndw", { 18, 18 }, 0, ImGuiWindowFlags_NoBackground);
        {
            ui::nav_btn();
        }
        ImGui::EndChild();

        ImGui::SetCursorPos({ 30 + ui::nav_width, 15 });
        ImGui::BeginChild("main", { ImGui::GetContentRegionAvail().x - 15, ImGui::GetContentRegionAvail().y - 15 }, 0, ImGuiWindowFlags_NoBackground);
        {
            if (!ui::tabs[ui::cur_tab].m_subtabs.empty()) {
                ImGui::BeginChild("subtabs", { ImGui::GetWindowWidth(), 51 }, 0, ImGuiWindowFlags_NoBackground);
                {
                    ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_ChildBg), ImGui::GetStyle().ChildRounding);
                    ImGui::GetWindowDrawList()->AddRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_Border), ImGui::GetStyle().ChildRounding, 0, ImGui::GetStyle().ChildBorderSize);

                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 8, 8 });
                    ImGui::SetCursorPos({ 15, 18 });
                    ImGui::BeginGroup();
                    {
                        for (int i = 0; i < ui::tabs[ui::cur_tab].m_subtabs.size(); ++i)
                        {
                            ui::subtab(i);
                            if (i != ui::tabs[ui::cur_tab].m_subtabs.size() - 1) {
                                ImGui::SameLine();
                                ui::vertical_separator(0, 15);
                                ImGui::SameLine();
                            }
                        }
                    }
                    ImGui::EndGroup();
                    ImGui::PopStyleVar();
                }
                ImGui::EndChild();

                ImGui::Dummy({ 0, 5 });
            }

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ui::anim);
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 10, 10 });
            ImGui::BeginChild("content", { ImGui::GetWindowWidth(), ImGui::GetContentRegionAvail().y }, 0, ImGuiWindowFlags_NoBackground);
            {
                ui::render_pages();
            }
            ImGui::EndChild();
            ImGui::PopStyleVar(2);
        }
        ImGui::EndChild();
    }
    ImGui::End();

    ui::anim = ImLerp(ui::anim, 1.f, ui::anim_speed);
    ui::size.x = ImLerp(ui::size.x, 584.f + ui::nav_width, ui::anim_speed);
    ui::nav_width = ImLerp(ui::nav_width, (ui::nav_opened ? 201.f : 66.f), ui::anim_speed);
}
#elif defined(CHINA)
void menufakehz()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // --- Light Color Theme ---
    ImVec4 mainBgColor = ImVec4(0.95f, 0.95f, 0.97f, 1.0f);  // main window background
    ImVec4 sidebarColor = ImVec4(0.92f, 0.92f, 0.95f, 1.0f);  // sidebar background
    ImVec4 headerColor = ImVec4(0.80f, 0.80f, 0.85f, 1.0f);   // header bar
    ImVec4 textColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);     // dark text
    ImVec4 headingColor = ImVec4(0.20f, 0.45f, 0.80f, 1.0f);  // heading color (slight blue tone)
    ImVec4 buttonColor = ImVec4(0.85f, 0.85f, 0.90f, 1.0f);
    ImVec4 buttonHovered = ImVec4(0.75f, 0.75f, 0.80f, 1.0f);
    ImVec4 buttonActive = ImVec4(0.65f, 0.65f, 0.70f, 1.0f);

    ImVec4 customSliderGrab = ImVec4(0.40f, 0.60f, 0.90f, 1.0f);
    ImVec4 customSliderGrabActive = ImVec4(0.30f, 0.50f, 0.80f, 1.0f);
    ImVec4 customCheckboxBg = ImVec4(0.85f, 0.85f, 0.90f, 1.0f);
    ImVec4 customCheckboxHovered = ImVec4(0.80f, 0.80f, 0.85f, 1.0f);
    ImVec4 customCheckboxActive = ImVec4(0.75f, 0.75f, 0.80f, 1.0f);
    ImVec4 customCheckMarkColor = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);

    // Scrollbar colors
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.70f, 0.70f, 0.75f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.60f, 0.60f, 0.65f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.55f, 1.0f);
    style.ScrollbarSize = 12.0f;

    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, mainBgColor);

    if (ImGui::Begin("CustomMenu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float headerHeight = 60.0f;
        float sidebarWidth = 170.0f;

        // Draw Header
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(windowPos, ImVec2(windowPos.x + windowSize.x, windowPos.y + headerHeight), ImColor(headerColor), 10.0f);

        ImGui::SetCursorPosY(headerHeight + 10);

        // Sidebar
        ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, windowSize.y - headerHeight - 20), true, ImGuiWindowFlags_None);
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActive);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        if (ImGui::Button("视觉效果", ImVec2(sidebarWidth - 20, 40))) current_tab = 0;
        if (ImGui::Button("瞄准机器人", ImVec2(sidebarWidth - 20, 40))) current_tab = 1;
        if (ImGui::Button("杂项", ImVec2(sidebarWidth - 20, 40))) current_tab = 2;

        ImGui::PopStyleColor(4);
        ImGui::EndChild();

        ImGui::SameLine();

        // Main Content Area
        ImGui::BeginChild("MainContent", ImVec2(windowSize.x - sidebarWidth - 30, windowSize.y - headerHeight - 20), true);

        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
        ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

        if (current_tab == 0)
        {
            ImGui::TextColored(headingColor, "视觉设置");
            ImGui::Separator();

            ImGui::BeginChild("VisualsScroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

            ImGui::Checkbox("方框", &settings::drawbox);
            ImGui::Checkbox("玩家姓名", &settings::playername);
            ImGui::Checkbox("绘制骨骼", &settings::skeleton);
            ImGui::Checkbox("绘制健康", &settings::healthESP);
            ImGui::Checkbox("绘制装甲", &settings::shieldESP);
            ImGui::Checkbox("绘制距离", &settings::drawdistance);
            ImGui::Checkbox("绘制机器人", &settings::showbots);
            ImGui::Checkbox("掩体判断", &settings::vischeck);
            ImGui::Checkbox("全面战场", &settings::teamcheck_war);
            ImGui::Checkbox("烽火地带", &settings::teamcheck_op);
            ImGui::SliderFloat("渲染距离", &settings::render_distance, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("渲染距离 BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::Checkbox("Show ground items", &settings::drawground_item);
            ImGui::Checkbox("Item Price", &settings::priceitem);
            ImGui::Checkbox("Show DeathBox", &settings::drawdeathbox);
            ImGui::SliderFloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);

            ImGui::EndChild();
        }
        else if (current_tab == 1)
        {
            ImGui::TextColored(headingColor, "机器人设定");
            ImGui::Separator();

            ImGui::BeginChild("AimbotScroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

            ImGui::Checkbox("启用瞄准机器人", &settings::aimbot);
            ImGui::Checkbox("预测弹道", &settings::prediction);
            ImGui::Checkbox("显示视场圈", &settings::show_fov);
            ImGui::SliderFloat("光滑度", &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("视场角", &settings::fov, 50, 300, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::Text("瞄准机器人 钥匙: %s", GetKeyNameAB(selected_key).c_str());

            if (awaiting_keybind)
            {
                ImGui::Text("按任意键...");
                for (int key = 1; key < 256; ++key)
                {
                    if (GetAsyncKeyState(key) & 0x8000)
                    {
                        selected_key = key;
                        awaiting_keybind = false;
                        break;
                    }
                }
            }
            else
            {
                if (ImGui::Button("设置按键绑定"))
                {
                    awaiting_keybind = true;
                }
            }

            ImGui::Combo("命中箱", &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));

            ImGui::EndChild();
        }
        else if (current_tab == 2)
        {
            ImGui::TextColored(headingColor, "其他设置");
            ImGui::Separator();

            ImGui::BeginChild("MiscScroll", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

            // Misc options here

            ImGui::EndChild();
        }

        ImGui::PopStyleColor(7);
        ImGui::EndChild();
    }

    ImGui::End();
    ImGui::PopStyleColor();
}

void menu4()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Buttons
    ImVec4 buttonColor1 = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    ImVec4 buttonHovered1 = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    ImVec4 buttonActive1 = ImVec4(0.45f, 0.45f, 0.45f, 1.0f);

    // --- Scrollbar Style ---
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.0f);
    style.ScrollbarSize = 11.0f;

    // Desired fixed window size
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);

    // --- Main Color Scheme ---
    ImVec4 headerColor = ImVec4(0.20f, 0.45f, 0.70f, 1.0f); // Vibrant blue header (drawn in background)
    ImVec4 mainBgColor = ImVec4(0.12f, 0.12f, 0.12f, 1.0f); // Dark gray background
    ImVec4 buttonColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f); // Button normal
    ImVec4 buttonHovered = ImVec4(0.35f, 0.35f, 0.35f, 1.0f); // Button hovered
    ImVec4 buttonActive = ImVec4(0.45f, 0.45f, 0.45f, 1.0f); // Button active
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);     // White text
    ImVec4 headingColor = ImVec4(0.0f, 0.85f, 0.85f, 1.0f);   // Bright teal for headings

    // --- Custom Slider & Checkbox Colors ---
    // Adjust these if you prefer different hues.
    ImVec4 customSliderGrab = ImVec4(0.9f, 0.2f, 0.2f, 1.0f);
    ImVec4 customSliderGrabActive = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
    ImVec4 customCheckboxBg = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 customCheckboxHovered = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 customCheckboxActive = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 customCheckMarkColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Apply the main window background color
    ImGui::PushStyleColor(ImGuiCol_WindowBg, mainBgColor);

    // Create the window without a title bar, collapse, or resize
    if (ImGui::Begin("uihbarname", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar))
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 windowSize = ImGui::GetWindowSize();
        float headerHeight = 50.0f;
        float sidebarWidth = 150.0f;

        // --- Draw "Header" Title (Centered) ---
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // We only add text (no filled rect) for the header, 
        // but you can fill a rectangle with 'headerColor' if desired.
        const char* title = "Delta Force";
        ImVec2 titleSize = ImGui::CalcTextSize(title);
        ImVec2 titlePos(
            windowPos.x + (windowSize.x - titleSize.x) * 0.5f,
            windowPos.y + (headerHeight - titleSize.y) * 0.5f
        );
        drawList->AddText(titlePos, IM_COL32(255, 255, 255, 255), title);

        // Vertical spacing below the "header"
        ImGui::SetCursorPosY(headerHeight + 10);

        // --- Sidebar (Left Pane) ---
        ImGui::BeginChild("Sidebar", ImVec2(sidebarWidth, windowSize.y - headerHeight - 20), true);

        // Push button + text colors for the tab buttons
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonActive);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor); // White text on buttons

        if (ImGui::Button("视觉效果", ImVec2(sidebarWidth - 20, 40)))
            current_tab = 0;
#ifdef ESPONLY
#else
        if (ImGui::Button("瞄准机器人", ImVec2(sidebarWidth - 20, 40)))
            current_tab = 1;
        if (ImGui::Button("杂项", ImVec2(sidebarWidth - 20, 40)))
            current_tab = 2;
#endif
        ImGui::PopStyleColor(4);
        ImGui::EndChild();

        ImGui::SameLine();

        // --- Main Content Area (Right Pane) ---
        ImGui::BeginChild("MainContent", ImVec2(windowSize.x - sidebarWidth - 20, windowSize.y - headerHeight - 20), true);

        // Tab content:
        if (current_tab == 0)  // Visuals Tab
        {
            ImGui::TextColored(headingColor, "视觉设置");
            ImGui::Separator();

            // Push custom style colors for text, sliders, and checkboxes
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::Checkbox(" 方框", &settings::drawbox);
            ImGui::Checkbox("绘制骨骼", &settings::skeleton);
            ImGui::Checkbox("绘制健康", &settings::healthESP);
            ImGui::Checkbox("绘制装甲", &settings::shieldESP);
            ImGui::Checkbox("绘制距离", &settings::drawdistance);

            ImGui::Checkbox("绘制机器人", &settings::showbots);

            ImGui::Checkbox("掩体判断", &settings::vischeck);
            ImGui::Checkbox("全面战场", &settings::teamcheck_war);
            ImGui::Checkbox("烽火地带", &settings::teamcheck_op);
            ImGui::SliderFloat("渲染距离", &settings::render_distance, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("渲染距离 BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

            ImGui::Checkbox("Show ground items", &settings::drawground_item);
            ImGui::Checkbox("Item Price", &settings::priceitem);
            ImGui::Checkbox("Show DeathBox", &settings::drawdeathbox);
            ImGui::SliderFloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);

            // Pop the 7 style colors we pushed
            ImGui::PopStyleColor(7);
        }
        else if (current_tab == 1)  // Aimbot Tab
        {
            ImGui::TextColored(headingColor, "机器人设定");
            ImGui::Separator();

            // Push style colors for text, sliders, and checkboxes
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::Checkbox("启用瞄准机器人", &settings::aimbot);
            ImGui::Checkbox("预测弹道", &settings::prediction);
            ImGui::Checkbox("显示视场圈", &settings::show_fov);
            ImGui::SliderFloat("光滑度", &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
            ImGui::SliderFloat("视场角", &settings::fov, 50, 300, "%.f", ImGuiSliderFlags_NoInput);

            // Display the current keybind
            ImGui::Text("瞄准机器人 钥匙: %s", GetKeyNameAB(selected_key).c_str());

            // Button to set a new keybind
            if (awaiting_keybind)
            {
                ImGui::Text("按任意键...");
                for (int key = 1; key < 256; ++key)
                {
                    if (GetAsyncKeyState(key) & 0x8000)
                    {
                        selected_key = key;
                        awaiting_keybind = false;
                        break;
                    }
                }
            }
            else
            {
                if (ImGui::Button("设置按键绑定"))
                {
                    awaiting_keybind = true;
                }
            }
            ImGui::Combo("命中箱", &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));

            ImGui::PopStyleColor(7);
        }
        else if (current_tab == 2)  // Misc Tab
        {
            ImGui::TextColored(headingColor, "其他设置");
            ImGui::Separator();

            // Push style colors for text, sliders, and checkboxes
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, customSliderGrab);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, customSliderGrabActive);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, customCheckboxBg);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, customCheckboxHovered);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, customCheckboxActive);
            ImGui::PushStyleColor(ImGuiCol_CheckMark, customCheckMarkColor);

            ImGui::PopStyleColor(7);
        }

        ImGui::EndChild();  // End MainContent
    }
    ImGui::End();

    // Pop the window background color
    ImGui::PopStyleColor();
}

#else
void drawmenu()
{
#define _(x) x
    static heads    category{ eye };
    static subheads subcategory{ aim };

    static bool checkbox = false;
    static int  slider, combo, key = 0;

    static float box_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::SetNextWindowSize({ 650, 420 }); {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 }); {
            ImGui::Begin(_("uihbarname"), nullptr, ImGuiWindowFlags_NoDecoration); {
                auto       draw = ImGui::GetWindowDrawList();
                ImVec2     pos = ImGui::GetWindowPos();
                ImVec2     size = ImGui::GetWindowSize();
                ImGuiStyle style = ImGui::GetStyle();

                // anims.
                anim = ImLerp(anim, (anim < 1.0f ? 1.0f : 0.0f), 0.20f * (1.0f - ImGui::GetIO().DeltaTime));
                expand_anim = ImLerp(expand_anim, (expand ? 1.0f : 0.0f), 0.10f * (1.0f - ImGui::GetIO().DeltaTime));

                // upper rect. {
                draw->AddRectFilled(ImVec2(pos.x + 68, pos.y), ImVec2(pos.x + size.x, pos.y + 53), ImColor(254, 254, 254), style.WindowRounding, ImDrawFlags_RoundCornersTopRight);
                draw->AddLine(ImVec2(pos.x + 68, pos.y + 53), ImVec2(pos.x + size.x, pos.y + 53), ImColor(0.0f, 0.0f, 0.0f, 0.10f)); // }

                // BRANDING.
                draw->AddText(ImVec2(pos.x + size.x - 10 - ImGui::CalcTextSize(_("GULFCHEATS")).x, pos.y + size.y - 7 - ImGui::CalcTextSize(_("GULFCHEATS")).y), ImColor(70, 70, 70), _("GULFCHEATS"));

                // categories. {
                switch (category) {
                case target:
                    ImGui::SetCursorPos({ 80, 14 }); {
                        ImGui::BeginGroup(); {

                        } ImGui::EndGroup();
                    }

                    switch (subcategory) {
                    case aim:
                        ImGui::SetCursorPos({ 80, 53 + (11 * anim) }); {
                            gui::beginchild(_("Aim assistance"), ImVec2(275, 330)); {
                                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim); {

                                    gui::checkbox(_("Enable Aimbot"), &settings::aimbot);
                                    gui::checkbox("Prediction", &settings::prediction);
                                    gui::checkbox(_("Show FOV Circle"), &settings::show_fov);
                                    gui::sliderfloat(_("Smoothness"), &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
                                    gui::sliderfloat(_("FOV"), &settings::fov, 50, 300, "%.f", ImGuiSliderFlags_NoInput);

                                } ImGui::PopStyleVar();
                            } gui::endchild();
                        }

                        ImGui::SetCursorPos({ 364, 53 + (11 * anim) }); {
                            gui::beginchild(_("Aim assistance 2"), ImVec2(275, 330)); {
                                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim); {

                                    // Display the current keybind
                                    ImGui::Text(_("Aimbot Key: %s"), GetKeyNameAB(selected_key).c_str());

                                    // Button to set a new keybind
                                    if (awaiting_keybind)
                                    {
                                        ImGui::Text(_("Press any key..."));
                                        for (int key = 1; key < 256; ++key)
                                        {
                                            if (GetAsyncKeyState(key) & 0x8000)
                                            {
                                                selected_key = key;
                                                awaiting_keybind = false;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (gui::button(_("Set Keybind")))
                                        {
                                            awaiting_keybind = true;
                                        }
                                    }
                                    gui::combo(_("Hitbox"), &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));

                                } ImGui::PopStyleVar();
                            } gui::endchild();
                        }
                        break;
                    }
                    break;
                case eye:
                    ImGui::SetCursorPos({ 80, 53 + (11 * anim) }); {
                        gui::beginchild(_("Visuals"), ImVec2(275, 330)); {
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim); {
                                gui::checkbox(_("Draw Box"), &settings::drawbox);
                                gui::checkbox(_("Draw Distance"), &settings::drawdistance);
                                gui::checkbox(_("Draw Skeleton"), &settings::skeleton);
                                gui::checkbox("Draw Health", &settings::healthESP);
                                gui::checkbox("Draw Name", &settings::playername);
                                gui::checkbox("Draw Armor", &settings::shieldESP);
                                gui::checkbox("Show Bots", &settings::showbots);


                                gui::sliderfloat(_("Render Distance"), &settings::render_distance, 1, 500, _("%.f"), ImGuiSliderFlags_NoInput);
                                gui::sliderfloat("Render Distance BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

                                gui::checkbox(_("Visible Check"), &settings::vischeck);
                                gui::checkbox("Team Check Warfare", &settings::teamcheck_war);
                                gui::checkbox("Team Check Operations", &settings::teamcheck_op);

                                gui::checkbox("Show ground items", &settings::drawground_item);
                                gui::checkbox("Item Price", &settings::priceitem);
                                gui::checkbox("Show DeathBox", &settings::drawdeathbox);
                                gui::sliderfloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
                                gui::sliderfloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);
                                 
                                if (ImGui::Button(isExcluded ? "Allow Capture" : "Block Capture"))
                                {
                                    isExcluded = !isExcluded;
                                    DWORD flag = isExcluded ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE;
                                    Drv->hide_window((uint64_t)hwnd);
                                }

                            } ImGui::PopStyleVar();
                        } gui::endchild();
                    }

                    ImGui::SetCursorPos({ 364, 53 + (11 * anim) }); {
                        gui::beginchild(_("Visuals Preview"), ImVec2(275, 330)); {
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim); {
                                // shadow
                                draw->AddShadowRect(ImVec2(pos.x + 430, pos.y + 114 + (11 * anim)), ImVec2(pos.x + 580, pos.y + 354 + (11 * anim)), ImColor(0, 0, 0, 180), 220.0f, ImVec2(0, 0));


                                if (settings::drawbox) {
                                    gui::default_box(draw, pos, ImColor(box_color[0], box_color[1], box_color[2]));
                                }

                                if (settings::drawdistance) {
                                    gui::distance(draw, pos);
                                }

                                if (settings::skeleton) {
                                    gui::skeleton(draw, pos);
                                }
                                /*
                                if (settings::head) {
                                    gui::headcircle(draw, pos);
                                }
                                if (settings::username) {
                                    gui::name(draw, pos);
                                }

                                if (settings::penis) {
                                    gui::penis(draw, pos);
                                }
                                */
                            } ImGui::PopStyleVar();
                        } gui::endchild();

                        // model.
                        ImGui::SetCursorPos({ 410, 102 + (11 * anim) }); {
                            ImGui::Image(valomodel, { 196, 257 });
                        }
                    }
                    break;

                case tools:
                    ImGui::SetCursorPos({ 80, 53 + (11 * anim) }); {
                        gui::beginchild(_("Misc"), ImVec2(275, 330)); {
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim); {

                            } ImGui::PopStyleVar();
                        } gui::endchild();
                    }

                    ImGui::SetCursorPos({ 364, 53 + (11 * anim) }); {
                        gui::beginchild(_(""), ImVec2(275, 330)); {
                            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, anim); {

                            } ImGui::PopStyleVar();
                        } gui::endchild();

                        // model.
                        ImGui::SetCursorPos({ 410, 102 + (11 * anim) }); {
                            ImGui::Image(valomodel, { 196, 257 });
                        }
                    }
                    break;

                } // }

                ImGui::SetCursorPos({ 0, 0 }); {
                    gui::expandchild(_("##categoriesCHILD"), ImVec2(67 + (100 * expand_anim), size.y)); {
                        // expand. {
                        ImGui::SetCursorPos({ 25 + (110 * expand_anim), 20 }); {
                            ImGui::BeginGroup(); {
                                if (gui::expandbutton((expand ? _("b") : _("a")), expand)) {
                                    expand = !expand;
                                }
                            } ImGui::EndGroup();
                        }
                        // }

                        // cheatname. {
                        if (expand) {
                            GetForegroundDrawList()->AddText(io.Fonts->Fonts[4], 19.0f, ImVec2(pos.x + 50 + (30 * expand_anim) - io.Fonts->Fonts[4]->CalcTextSizeA(19.0f, FLT_MAX, 0.0f, _("GULFCHEATS")).x / 2, pos.y + 20), ImColor(70, 70, 70), _("GULF"));
                            GetForegroundDrawList()->AddText(io.Fonts->Fonts[4], 19.0f, ImVec2(pos.x + 50 + (30 * expand_anim) - io.Fonts->Fonts[4]->CalcTextSizeA(19.0f, FLT_MAX, 0.0f, _("GULFCHEATS")).x / 2 + io.Fonts->Fonts[4]->CalcTextSizeA(19.0f, FLT_MAX, 0.0f, _("GULF")).x, pos.y + 20), ImColor(accent_c[0], accent_c[1], accent_c[2]), _("CHEATS"));
                        }
                        // }
#ifdef ESPONLY
#else
                        // categories render. {
                        ImGui::SetCursorPos({ 21 + (16 * expand_anim), 65 }); {
                            ImGui::BeginGroup(); {
#ifdef ESPONLY
#else
                                if (gui::category(_("A"), _("Aimbot"), category == target)) { category = target; anim = 0.0f; }
#endif
                            } ImGui::EndGroup();
                        }

                        if (expand)
                            GetForegroundDrawList()->AddLine(ImVec2(pos.x + 22, pos.y + 172), ImVec2(pos.x + 45 + (100 * expand_anim), pos.y + 172), ImColor(0.0f, 0.0f, 0.0f, 0.17f));
                        else
                            draw->AddLine(ImVec2(pos.x + 22, pos.y + 172), ImVec2(pos.x + 45 + (100 * expand_anim), pos.y + 172), ImColor(0.0f, 0.0f, 0.0f, 0.17f));
#endif
                        ImGui::SetCursorPos({ 21 + (16 * expand_anim), 187 }); {
                            ImGui::BeginGroup(); {
                                if (gui::category(_("D"), _("Visuals"), category == eye)) { category = eye; anim = 0.0f; }
                            } ImGui::EndGroup();
                        }

                        if (expand)
                            GetForegroundDrawList()->AddLine(ImVec2(pos.x + 22, pos.y + 294), ImVec2(pos.x + 45 + (100 * expand_anim), pos.y + 294), ImColor(0.0f, 0.0f, 0.0f, 0.17f));
                        else
                            draw->AddLine(ImVec2(pos.x + 22, pos.y + 294), ImVec2(pos.x + 45 + (100 * expand_anim), pos.y + 294), ImColor(0.0f, 0.0f, 0.0f, 0.17f));

                        ImGui::SetCursorPos({ 21 + (16 * expand_anim), 307 }); {
                            ImGui::BeginGroup(); {
                                if (gui::category(_("G"), _("Misc"), category == tools)) { category = tools; anim = 0.0f; }
                            } ImGui::EndGroup();
                        } // }
                    } gui::endexpandchild();
                }
            } ImGui::End();
        } ImGui::PopStyleVar();
    }
}

#endif

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void renderfunction();

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void initimguirendershits()
{
    // Get screen size
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("class04"), NULL };
    ::RegisterClassEx(&wc);

    hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        wc.lpszClassName,
        "classui04",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        NULL, NULL, wc.hInstance, NULL
    );

    while (!hwnd)
        SleepEx(1, false);

    //Drv->HideWindow(hwnd, WDA_EXCLUDEFROMCAPTURE);
    HideWindow((uint64_t)hwnd, WDA_EXCLUDEFROMCAPTURE);

    // Extend frame for visual transparency
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    // Set alpha to 255 (fully opaque)
    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.IniFilename = NULL;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

#ifdef NotShxdowBrand
    auto config = ImFontConfig();
    config.FontDataOwnedByAtlas = false;

    io.Fonts->AddFontFromMemoryTTF((void*)stolzl_regular_bin, sizeof(stolzl_regular_bin), 15.f, &config);
    //io.Fonts->AddFontFromMemoryTTF((void*)font_awesome_bin, sizeof(font_awesome_bin), 16.f, &icons_config, icon_ranges);
    io.Fonts->AddFontFromMemoryTTF((void*)stolzl_medium_bin, sizeof(stolzl_medium_bin), 15.f, &config);
    io.Fonts->AddFontFromMemoryTTF((void*)stolzl_regular_bin, sizeof(stolzl_regular_bin), 14.f, &config);
    io.Fonts->AddFontFromMemoryTTF((void*)stolzl_medium_bin, sizeof(stolzl_medium_bin), 16.f, &config);

    for (int i = 0; i < 4; ++i) {
        float cols[4] = { ImGui::GetStyleColorVec4(ImGuiCol_Scheme).x, ImGui::GetStyleColorVec4(ImGuiCol_Scheme).y, ImGui::GetStyleColorVec4(ImGuiCol_Scheme).z, ImGui::GetStyleColorVec4(ImGuiCol_Scheme).w };
        ui::accent_col_holder[i] = { cols[i] };
    }

    ui::add_page(0, []()
        {
            static bool bools[64];
            static int ints[64];
            static float floats[64];
            static char buf[64];

            ImGui::BeginGroup();
            {
                ui::begin_child("AIM 1", { ImGui::GetWindowWidth() / 2 - ImGui::GetStyle().ItemSpacing.x / 2, ImGui::GetWindowHeight() });
                {
                    ImGui::Checkbox("Enable Aimbot", &settings::aimbot);
                    ImGui::Checkbox("Prediction", &settings::prediction);

                    ImGui::Checkbox("Show FOV Circle", &settings::show_fov);
                    ImGui::SliderFloat("Smoothness", &settings::smoothness, 1, 50, "%.f", ImGuiSliderFlags_NoInput);
                    ImGui::SliderFloat("FOV", &settings::fov, 50, 300, "%.f", ImGuiSliderFlags_NoInput);

                }
                ui::end_child();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ui::begin_child("AIM 2", { ImGui::GetWindowWidth() / 2 - ImGui::GetStyle().ItemSpacing.x / 2, ImGui::GetWindowHeight() });
                {
                    // Display the current keybind
                    ImGui::Text("Aimbot Key: %s", GetKeyNameAB(selected_key).c_str());

                    // Button to set a new keybind
                    if (awaiting_keybind)
                    {
                        ImGui::Text("Press any key...");
                        for (int key = 1; key < 256; ++key)
                        {
                            if (GetAsyncKeyState(key) & 0x8000)
                            {
                                selected_key = key;
                                awaiting_keybind = false;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if (ImGui::Button("Set Keybind"))
                        {
                            awaiting_keybind = true;
                        }
                    }
                    ImGui::Combo("Hitbox", &settings::hitbox, settings::hitbox_items, IM_ARRAYSIZE(settings::hitbox_items));
                }
                ui::end_child();
            }
            ImGui::EndGroup();
        });

    ui::add_page(0, []()
        {
            ImGui::BeginGroup();
            {
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
            }
            ImGui::EndGroup();
        });

    ui::add_page(1, []()
        {
            static bool bools[64];
            static int ints[64];
            static float floats[64];
            static char buf[64];

            ImGui::BeginGroup();
            {
                ui::begin_child("ESP 1", { ImGui::GetWindowWidth() / 2 - ImGui::GetStyle().ItemSpacing.x / 2, ImGui::GetWindowHeight() });
                {
                    ImGui::Checkbox("Draw Box", &settings::drawbox);
                    ImGui::Checkbox("Draw Skeleton", &settings::skeleton);
                    ImGui::Checkbox("Draw Health", &settings::healthESP);
                    ImGui::Checkbox("Draw Name", &settings::playername);
                    ImGui::Checkbox("Draw Distance", &settings::drawdistance);
                    ImGui::Checkbox("Draw Armor", &settings::shieldESP);
                    ImGui::Checkbox("Draw Bots", &settings::showbots);
                    ImGui::Checkbox("Show ground items", &settings::drawground_item);
                    ImGui::Checkbox("Item Price", &settings::priceitem);
                    ImGui::Checkbox("Show DeathBox", &settings::drawdeathbox);
                }
                ui::end_child();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ui::begin_child("ESP 2", { ImGui::GetWindowWidth() / 2 - ImGui::GetStyle().ItemSpacing.x / 2, ImGui::GetWindowHeight() });
                {
                    ImGui::Checkbox("Visible Check", &settings::vischeck);
                    ImGui::Checkbox("Team Check War", &settings::teamcheck_war);
                    ImGui::Checkbox("Team Check Op", &settings::teamcheck_op);
                    ImGui::SliderFloat("Render Distance", &settings::render_distance, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
                    ImGui::SliderFloat("Render Distance BOT", &settings::render_distance_bot, 1, 500, "%.f", ImGuiSliderFlags_NoInput);

                    ImGui::SliderFloat("Item Max Distance", &settings::maxitemdist, 1, 500, "%.f", ImGuiSliderFlags_NoInput);
                    ImGui::SliderFloat("Item Min Value", &settings::max_item_value, 0, 100000, "%.f", ImGuiSliderFlags_NoInput);
                }
                ui::end_child();
            }
            ImGui::EndGroup();
        });

    ui::add_page(1, []()
        {
            ImGui::BeginGroup();
            {
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
            }
            ImGui::EndGroup();
        });

    ui::add_page(2, []()
        {
            static bool bools[64];
            static int ints[64];
            static float floats[64];
            static char buf[64];

            ImGui::BeginGroup();
            {
                ui::begin_child("MISC 1", { ImGui::GetWindowWidth() / 2 - ImGui::GetStyle().ItemSpacing.x / 2, ImGui::GetWindowHeight() });
                {

                }
                ui::end_child();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                ui::begin_child("MISC 2", { ImGui::GetWindowWidth() / 2 - ImGui::GetStyle().ItemSpacing.x / 2, ImGui::GetWindowHeight() });
                {

                }
                ui::end_child();
            }
            ImGui::EndGroup();
        });

    ui::add_page(2, []()
        {
            ImGui::BeginGroup();
            {
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
            }
            ImGui::EndGroup();
        });

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

#else
// Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

#ifdef CHINA
    //ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simsun.ttc", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(test_symbol_compressed_data, test_symbol_compressed_size, 14.f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
#else
#ifdef AXADMIN
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahomabd.ttf", 13.0f, nullptr, io.Fonts->GetGlyphRangesThai()); // ไทย
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;         // รวมเข้ากับฟอนต์ก่อนหน้า
    fontConfig.PixelSnapH = true;        // ปรับตำแหน่งพิกเซลให้คม
    io.Fonts->AddFontFromMemoryTTF((void*)SimHei_data, sizeof(SimHei_data), 13.0f, &fontConfig, io.Fonts->GetGlyphRangesChineseFull());
#elif defined(GAMELORD)
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\tahomabd.ttf", 13.0f, nullptr, io.Fonts->GetGlyphRangesThai()); // ไทย
    ImFontConfig fontConfig;
    fontConfig.MergeMode = true;         // รวมเข้ากับฟอนต์ก่อนหน้า
    fontConfig.PixelSnapH = true;        // ปรับตำแหน่งพิกเซลให้คม
    io.Fonts->AddFontFromMemoryTTF((void*)SimHei_data, sizeof(SimHei_data), 13.0f, &fontConfig, io.Fonts->GetGlyphRangesChineseFull());
#else
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    io.Fonts->AddFontFromMemoryTTF(MiniCondensedRegular, sizeof(MiniCondensedRegular), 14.0f, &font_config, ranges);
    io.Fonts->AddFontFromMemoryTTF(Icons, sizeof(Icons), 12.0f, &font_config, ranges);
    io.Fonts->AddFontFromMemoryTTF(ComboArrow, sizeof(ComboArrow), 7.0f, &font_config, ranges);
    io.Fonts->AddFontFromMemoryTTF(ExpandIcon, sizeof(ExpandIcon), 20.0f, &font_config, ranges);

    io.Fonts->AddFontFromMemoryTTF(MiniCondensedRegular, sizeof(MiniCondensedRegular), 19.0f, &font_config, ranges);
#endif
#endif
#endif

    renderfunction();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void DrawMouseDot() {
    // Get current mouse position
    ImVec2 mousePos = ImGui::GetMousePos();

    // Access the current ImGui drawing list
    ImDrawList* drawList = ImGui::GetForegroundDrawList();

    // Draw a small filled circle at the mouse position
    float dotRadius = 4.0f; // Radius of the dot
    ImU32 dotColor = IM_COL32(255, 0, 0, 255); // Red color (RGBA)

    drawList->AddCircleFilled(mousePos, dotRadius, dotColor);
}

void draw_box(int x, int y, int w, int h, const ImColor color, ImColor fill_color, int thickness)
{
    ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
}

void draw_text(ImFont* font, float fontSize, ImVec2 position, ImColor color, const char* text)
{
    ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y - 1), ImColor(0, 0, 0), text);
    ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y - 1), ImColor(0, 0, 0), text);
    ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x - 1, position.y + 1), ImColor(0, 0, 0), text);
    ImGui::GetForegroundDrawList()->AddText(font, fontSize, ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0), text);
    ImGui::GetForegroundDrawList()->AddText(font, fontSize, position, color, text);
}

void DrawOutlinedLine(int x1, int y1, int x2, int y2, ImU32 color, int thickness) {
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImColor(0, 0, 0, 255), thickness + 2); // Outline
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness); // Main line
}

enum struct PlayerBone : uint8_t {
    Root = 0,
    Hips = 1,
    Spine = 2,
    Spine1 = 3,
    Spine2 = 4,
    RightShoulder = 5,
    RightArm = 6,
    RightForeArm = 7,
    RightHand = 10,
    LeftShoulder = 33,
    LeftArm = 34,
    LeftForeArm = 35,
    LeftHand = 38,
    Neck = 30,
    Head = 31,
    LeftUpLeg = 58,
    LeftLeg = 59,
    LeftFoot = 60,
    RightUpLeg = 62,
    RightLeg = 63,
    RightFoot = 64
};

struct Connection {
    PlayerBone a, b;
};

bool clipLine(float& x0, float& y0, float& x1, float& y1, float x_min, float y_min, float x_max, float y_max) {
    float p[4] = { -(x1 - x0), x1 - x0, -(y1 - y0), y1 - y0 };
    float q[4] = { x0 - x_min, x_max - x0, y0 - y_min, y_max - y0 };
    float u1 = 0.0f, u2 = 1.0f;

    for (int i = 0; i < 4; i++) {
        if (p[i] == 0) {
            // Line is parallel to clipping boundary.
            if (q[i] < 0)
                return false;
        }
        else {
            float t = q[i] / p[i];
            if (p[i] < 0) {
                if (t > u2)
                    return false;
                if (t > u1)
                    u1 = t;
            }
            else {
                if (t < u1)
                    return false;
                if (t < u2)
                    u2 = t;
            }
        }
    }
    if (u2 < u1)
        return false;

    float nx0 = x0 + u1 * (x1 - x0);
    float ny0 = y0 + u1 * (y1 - y0);
    float nx1 = x0 + u2 * (x1 - x0);
    float ny1 = y0 + u2 * (y1 - y0);

    x0 = nx0; y0 = ny0; x1 = nx1; y1 = ny1;
    return true;
}

void skeleton(uint64_t actorMesh, ImColor color, float boxX, float boxY, float boxW, float boxH, uintptr_t BoneArray) {
    // List of unique bones used in the skeleton
    static const PlayerBone uniqueBones[] = {
        PlayerBone::Head,
        PlayerBone::Neck,
        PlayerBone::Hips,
        PlayerBone::LeftUpLeg,
        PlayerBone::RightUpLeg,
        PlayerBone::LeftLeg,
        PlayerBone::RightLeg,
        PlayerBone::LeftFoot,
        PlayerBone::RightFoot,
        PlayerBone::LeftShoulder,
        PlayerBone::RightShoulder,
        PlayerBone::LeftArm,
        PlayerBone::RightArm,
        PlayerBone::LeftForeArm,
        PlayerBone::RightForeArm,
        PlayerBone::LeftHand,
        PlayerBone::RightHand
    };

    std::vector<Vector2> screenPositions(100, { 0, 0 });
    for (PlayerBone bone : uniqueBones) {
        int idx = static_cast<int>(bone);
        Vector3 boneWorldPos = GetEntityBone(actorMesh, BoneArray, idx);
        screenPositions[idx] = ProjectWorldToScreen(Pointer::CameraCache.POV, boneWorldPos);
    }

    static const Connection connections[] = {
        // Head & Neck
        { PlayerBone::Head, PlayerBone::Neck },
        { PlayerBone::Neck, PlayerBone::Hips },

        // Arms & Shoulders
        { PlayerBone::LeftShoulder, PlayerBone::LeftArm },
        { PlayerBone::RightShoulder, PlayerBone::RightArm },
        { PlayerBone::LeftArm, PlayerBone::LeftForeArm },
        { PlayerBone::LeftForeArm, PlayerBone::LeftHand },
        { PlayerBone::RightArm, PlayerBone::RightForeArm },
        { PlayerBone::RightForeArm, PlayerBone::RightHand },

        // Hips to Legs
        { PlayerBone::Hips, PlayerBone::LeftUpLeg },
        { PlayerBone::Hips, PlayerBone::RightUpLeg },
        { PlayerBone::LeftUpLeg, PlayerBone::LeftLeg },
        { PlayerBone::RightUpLeg, PlayerBone::RightLeg },
        { PlayerBone::LeftLeg, PlayerBone::LeftFoot },
        { PlayerBone::RightLeg, PlayerBone::RightFoot }
    };

    // Calculate the clipping rectangle boundaries
    float x_min = boxX;
    float y_min = boxY;
    float x_max = boxX + boxW;
    float y_max = boxY + boxH;

    for (const Connection& conn : connections) {
        int idxA = static_cast<int>(conn.a);
        int idxB = static_cast<int>(conn.b);
        const Vector2& posA = screenPositions[idxA];
        const Vector2& posB = screenPositions[idxB];

        // Make local copies to allow clipping
        float x0 = posA.x, y0 = posA.y, x1 = posB.x, y1 = posB.y;
        if (clipLine(x0, y0, x1, y1, x_min, y_min, x_max, y_max)) {
            DrawOutlinedLine(x0, y0, x1, y1, color, 1);
        }
    }
}

void DrawESPBox(ImDrawList* drawList, Vector2 screenPos, float width, float height, ImU32 color, float thickness) {
    drawList->AddRect(ImVec2(screenPos.x - width / 2, screenPos.y - height / 2),
        ImVec2(screenPos.x + width / 2, screenPos.y + height / 2), color, 0.0f, 0, thickness);
}

void DrawESPText(ImDrawList* drawList, Vector2 pos, const std::string& text, ImU32 color) {
    drawList->AddText(ImVec2(pos.x, pos.y), color, text.c_str());
}

void RenderESP(Vector2 screenPos, float distance, std::string itemName, ImColor color, int price) {
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    // Keep the box small regardless of distance
    float boxWidth = 5.0f;
    float boxHeight = 5.0f;

    ImU32 boxColor = color;
    float thickness = 0.5f;

    // Draw ESP Box
    if (settings::itembox)
        DrawESPBox(drawList, screenPos, boxWidth, boxHeight, boxColor, thickness);

    // Draw Item Name above the box, centered
    if (settings::itemname) {
        float nameWidth = ImGui::CalcTextSize(itemName.c_str()).x;
        DrawESPText(drawList, Vector2(screenPos.x - nameWidth / 2, screenPos.y - boxHeight / 2 - 15), itemName, color);
    }

    // Combine distance and price into one string
    if (settings::itemdist || settings::priceitem) {
        std::string combinedText;

        if (settings::itemdist)
            combinedText += std::to_string((int)distance) + "m";

        if (settings::priceitem && price > 0) {
            if (!combinedText.empty())
                combinedText += " | ";
            combinedText += "$" + std::to_string(price);
        }

        float combinedWidth = ImGui::CalcTextSize(combinedText.c_str()).x;
        DrawESPText(drawList, Vector2(screenPos.x - combinedWidth / 2, screenPos.y + boxHeight / 2 + 5), combinedText, color);
    }
}

// Health bar drawing function remains unchanged.
void DrawHealthBar(float x, float y, float width, float height, float health, float maxHealth)
{
    // Calculate filled health height based on current health.
    float healthHeight = (health / maxHealth) * height;
    ImU32 healthColor = ImColor(0, 255, 0, 255);      // Green for current health.
    ImU32 backgroundColor = ImColor(255, 0, 0, 255);    // Red for missing health.

    // Draw the full background of the health bar.
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), backgroundColor);

    // Draw the current health portion from the bottom up.
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y + (height - healthHeight)), ImVec2(x + width, y + height), healthColor);
}
// Draws a vertical bar representing the shield (or any secondary stat)
// The shield bar is drawn flush against the right side of the ESP box.
void DrawShieldBar(float x, float y, float width, float height, float shield, float maxShield)
{
    // Calculate the filled shield height based on the current shield value.
    float shieldHeight = (shield / maxShield) * height;
    ImU32 shieldColor = ImColor(0, 0, 255, 255);         // Blue for current shield.
    ImU32 shieldBackground = ImColor(100, 100, 100, 255);   // Gray for missing shield.

    // Draw the full background for the shield bar.
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + width, y + height), shieldBackground);

    // Draw the current shield portion from the bottom up.
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y + (height - shieldHeight)), ImVec2(x + width, y + height), shieldColor);
}