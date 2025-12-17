#include "includes.h"

void renderesp()
{
    Pointer::closest_distance = FLT_MAX;
    Pointer::closest_mesh = NULL;
    Pointer::closest_entity = NULL;
    Pointer::closest_entityroot_comp = NULL;
    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    Pointer::CameraCache.POV = read<FMinimalViewInfo>(Pointer::PlayerCameraManager + Offsets::Viewtarget + 0x10);
    //printf("FOV: %f\n", Pointer::CameraCache.POV.FOV);
//printf("Loc: %f:%f\n", Pointer::CameraCache.POV.Location.X, Pointer::CameraCache.POV.Location.Y);

    if (settings::show_fov)
    {
        ImGui::GetForegroundDrawList()->AddCircle(ImVec2(screen_center_x, screen_center_y), settings::fov, ImColor(0, 0, 0, 255), 100, 2.0f);
        ImGui::GetForegroundDrawList()->AddCircle(ImVec2(screen_center_x, screen_center_y), settings::fov, ImColor(255, 255, 255, 255), 100, 1.0f);
    }

    std::vector<objItemInf> localItem_entities;
    {
        std::lock_guard<std::mutex> lock(cacheMtx2);
        localItem_entities = get_items;
    }

    if (localItem_entities.empty()){}
    else
    {
        if(!wegame)
        {
            for (auto& entityItem : localItem_entities)
            {
                if (!entityItem.entityaddy)
                    continue;

                auto RootComponent = decrypt_pointer_simple(read<uint64_t>(entityItem.entityaddy + Offsets::RootComponent));
                auto Location = read<Vector3>(RootComponent + Offsets::RelativeLocation);

                Vector2 Screen = ProjectWorldToScreen(Pointer::CameraCache.POV, Location);
                if (Screen.x <= 0 || Screen.y <= 0 || Screen.x >= screenWidth || Screen.y >= screenHeight)
                    continue;

                float distance = Pointer::CameraCache.POV.Location.distance(Location) * 0.01f;
                if (distance >= settings::maxitemdist)
                    continue;

                auto ExtractName = [&](uintptr_t ptr) -> std::string {
                    static const int offsets[] = { 0x20, 0x40, 0x20 };
                    for (int i = 0; i < 3 && !IsInvalidPtr(ptr); ++i)
                        ptr = read<uintptr_t>(ptr + offsets[i]);
                    return WideToUTF8(ReadFStringW(ptr));
                    };

                ImColor color = ImColor(255, 255, 255, 255);

                auto DrawText2 = [&](const char* text, ImColor col, float yOffset = 0.0f) {
                    draw_text(ImGui::GetFont(), 13.0f, ImVec2(Screen.x, Screen.y + yOffset), col, text);
                    };

                switch (entityItem.MarkingItemType) {
                case EMarkingItemType::LootingItem: {
                    auto pickup = read<uint64_t>(entityItem.entityaddy + Offsets::CommonItemRow);
                    if (!pickup) break;

                    int price = read<int>(pickup + Offsets::FDFMCommonItemRow::GuidePrice);
                    int quality = read<int>(pickup + Offsets::FDFMCommonItemRow::Quality);
                    color = GetArmorColor(quality);
                    if (price <= 0 || price < settings::max_item_value ||
                        (color == ImColor(255, 255, 255, 255) && price > 2000)) break;

                    std::string name = ExtractName(read<uintptr_t>(pickup + 0x18));
                    if (name.empty()) name = "Unknown";

                    if (settings::drawground_item) {
                        char text[64];
                        snprintf(text, sizeof(text), "%s: [%d%s]", name.c_str(), (int)distance, "m");
                        DrawText2(text, color);
                    }
                    if (settings::priceitem) {
                        char priceText[64];
                        snprintf(priceText, sizeof(priceText), "%s%d%s", "Price: ", price, "$");
                        DrawText2(priceText, color, 14);
                    }
                    break;
                }

                case EMarkingItemType::CommonItem: {
                    int pwd = read<int>(entityItem.entityaddy + Offsets::Password);
                    if (pwd < 100) break;

                    if (settings::drawground_item) {
                        char text[64];
                        snprintf(text, sizeof(text), "%s[%d%s]\n%s%d",
                            "Computer: ", (int)distance, "m",
                            "Password: ", pwd);
                        DrawText2(text, color);
                    }
                    break;

                }

                case EMarkingItemType::Container: {
                    auto pickup = read<uint64_t>(entityItem.entityaddy + Offsets::CommonItemRow);
                    if (!pickup) break;

                    std::string name = ExtractName(read<uintptr_t>(pickup + 0x18));
                    if (name.empty()) name = "Container: ";

                    if (settings::drawground_item) {
                        char text[64];
                        snprintf(text, sizeof(text), "%s[%d%s]", name.c_str(), (int)distance, "m");
                        DrawText2(text, ImColor(255, 100, 0, 255));
                    }
                    break;

                }

                case EMarkingItemType::DeadBody: {
                    if (distance > 50) break;
                    if (settings::drawdeathbox) {
                        char text[64];
                        snprintf(text, sizeof(text), "%s[%d%s]", "Death Box: ", (int)distance, "m");
                        DrawText2(text, ImColor(255, 230, 0, 255));
                    }
                    break;
                }
                }
            }
        }
    }

    std::vector<objInf> local_entities;
    {
        std::lock_guard<std::mutex> lock(cacheMtx);
        local_entities = get_entities;
    }

    if (local_entities.empty()){}
    else
    {
        for (auto& entity : local_entities)
        {
            if (!entity.entityaddy || !entity.Mesh || !entity.BoneArray)
                continue;

            float playerhealth = gethealth(entity.entityaddy, false);
            if (playerhealth <= 1) continue;

            bool visible = is_visible(entity.Mesh);

            ImColor boxColor;
            if (visible) {
                boxColor = ImColor(255, 255, 255, 255);
                //if (entity.isBot)
                    //boxColor = ImColor(0, 0, 255, 255);
            }
            else
            {
                boxColor = ImColor(255, 0, 0, 255);
                //if (entity.isBot)
                    //boxColor = ImColor(255, 255, 0, 255);
            }

            Vector3 headPos = GetEntityBone(entity.Mesh, entity.BoneArray, Head);
            Vector3 rootPos = GetEntityBone(entity.Mesh, entity.BoneArray, 0);

            //printf("Loc: %f:%f\n", headPos.x, headPos.y);

            float distance = Pointer::CameraCache.POV.Location.distance(headPos) * 0.01f;

            if (!entity.isBot)
            {
                if (distance > settings::render_distance_bot) continue;
            }
            else
            {
                if (distance > settings::render_distance) continue;
            }

            Vector2 headScreen = ProjectWorldToScreen(Pointer::CameraCache.POV, headPos);
            Vector2 rootScreen = ProjectWorldToScreen(Pointer::CameraCache.POV, rootPos);

            if (rootScreen.x <= 0 || rootScreen.y <= 0 || rootScreen.x >= screenWidth || rootScreen.y >= screenHeight)
                continue;

            float height = abs(headScreen.y - rootScreen.y) * 1.05f;
            float width = height / 2.2f;

            float x = rootScreen.x - width / 2;
            float y = headScreen.y - (height * 0.02f);

            if (settings::playername)
            {
                ImVec2 nameSize = ImGui::CalcTextSize(entity.playername.c_str());
                float nameX = rootScreen.x - nameSize.x / 2;  // Center alignment
                float nameY = headScreen.y - 20.0f;  // Above head

                // Optional: you can choose color depending on team or visibility.
                //ImColor nameColor = ImColor(255, 255, 255, 255);
                ImColor nameColor = entity.TeamColor;
                draw_text(ImGui::GetFont(), 13.0f, ImVec2(nameX, nameY), nameColor, entity.playername.c_str());
            }

            if (settings::shieldESP)
            {
                ARMOR_INFO PlayerArmor = {};
                // Reading armor data
                uintptr_t equipcomp = read<uintptr_t>(entity.entityaddy + Offsets::CharacterEquipComponent);
                //printf("equipcomp: %u\n", equipcomp);
                //if (!equipcomp) continue;
                uintptr_t eqdata = read<uintptr_t>(equipcomp + Offsets::EquipmentInfoArray);
                //printf("eqdata: %u\n", eqdata);

                //if (!eqdata) continue;
                constexpr int ARMOR_SLOT_COUNT = 6;
                FEquipmentInfo ArmorInfoArray[ARMOR_SLOT_COUNT];
                if (ReadMemory((PVOID)eqdata, ArmorInfoArray, ARMOR_SLOT_COUNT * sizeof(FEquipmentInfo)))
                {
                    if (ArmorInfoArray[1].ItemID || ArmorInfoArray[1].Durability || ArmorInfoArray[5].ItemID || ArmorInfoArray[5].Durability)
                    {
                        int helmetlevel = GetArmorLevel(ArmorInfoArray[1].ItemID);
                        int chestlevel = GetArmorLevel(ArmorInfoArray[5].ItemID);

                        float HelmetDurability = ArmorInfoArray[1].MaxDurability;
                        float chestdurability = ArmorInfoArray[5].MaxDurability;

                        PlayerArmor.HelmetLevel = helmetlevel;
                        PlayerArmor.BreastPlateLevel = chestlevel;
                        PlayerArmor.BreastMaxDurability = chestdurability;
                        PlayerArmor.HelmetMaxDurability = HelmetDurability;
                    }
                }

                char armorText[64] = { 0 };
                char helmetText[64] = { 0 };

                snprintf(armorText, sizeof(armorText), "Chest: %.0f", PlayerArmor.BreastMaxDurability);
                snprintf(helmetText, sizeof(helmetText), "Helmet: %.0f", PlayerArmor.HelmetMaxDurability);

                // Calculate text positions: right of the box, vertically centered
                ImVec2 armor_size = ImGui::CalcTextSize(armorText);
                ImVec2 helmet_size = ImGui::CalcTextSize(helmetText);

                float totalTextHeight = armor_size.y + helmet_size.y;
                float startY = y + (height / 2) - (totalTextHeight / 2);

                float textX = x + width + 5.0f; // 5px to the right of the box
                float armorY = startY;
                float helmetY = startY + armor_size.y;

                // Get armor colors based on levels
                ImColor chestColor = GetArmorColor(PlayerArmor.BreastPlateLevel);
                ImColor helmetColor = GetArmorColor(PlayerArmor.HelmetLevel);

                draw_text(ImGui::GetFont(), 13.0f, ImVec2(textX, armorY), chestColor, armorText);
                draw_text(ImGui::GetFont(), 13.0f, ImVec2(textX, helmetY), helmetColor, helmetText);
            }

            if (settings::drawbox)
                draw_box(x, y, width, height, boxColor, 0, 2);

            if (settings::drawdistance)
            {
                float yOffsetBottom = 10.0f;
                char dist[64];
                sprintf_s(dist, "%.fm", distance);

                ImVec2 text_size = ImGui::CalcTextSize(dist);
                ImVec2 pos = ImVec2(rootScreen.x - text_size.x / 2, rootScreen.y + yOffsetBottom - text_size.y / 2);
                ImVec2 background_size = ImVec2(text_size.x + 10.0f, text_size.y);
                ImVec2 background_pos = ImVec2(pos.x - 5.0f, pos.y);

                //if (settings::outline) ImGui::GetForegroundDrawList()->AddRectFilled(background_pos, ImVec2(background_pos.x + background_size.x, background_pos.y + background_size.y), ImColor(0, 0, 0, 200), 5.0f);
                draw_text(ImGui::GetFont(), 13.0f, pos, ImColor(255, 255, 255), dist);

                yOffsetBottom += 15.0f;
            }

            if (settings::skeleton)
            {
                skeleton(entity.Mesh, boxColor, x, y, width, height, entity.BoneArray);
            }

            if (settings::healthESP)
            {
                float playerhealthMax = gethealth(entity.entityaddy, true);

                // Use the same height as the ESP box.
                float barHeight = height;
                float barWidth = 3.0f;  // Fixed width for the health bar.

                // Introduce a margin to avoid overlapping with the box lines.
                float margin = 3.0f;

                // Position the health bar to the left of the ESP box with the margin.
                float barX = x - barWidth - margin;
                float barY = y;  // Align top with the ESP box.

                // Draw the health bar flush and aligned with the box.
                DrawHealthBar(barX, barY, barWidth, barHeight, playerhealth, playerhealthMax);
            }

            if (settings::aimbot)
            {
                auto cross_dist = get_cross_distance(headScreen.x, headScreen.y, screen_center_x, screen_center_y);
                if (cross_dist <= settings::fov && cross_dist <= Pointer::closest_distance)
                {
                    Pointer::closest_distance = cross_dist;
                    Pointer::closest_mesh = entity.Mesh;
                    Pointer::closest_entity = entity.entityaddy;
                    Pointer::closest_bonearray = entity.BoneArray;
                }
            }
        }
    }

    if (settings::aimbot && (GetAsyncKeyState(selected_key) & 0x8000))
    {
        aimbot(Pointer::closest_mesh, Pointer::closest_entity, Pointer::closest_entityroot_comp, Pointer::closest_bonearray);
    }
}

void renderfunction()
{
    bool show_another_window = true;
    ImVec4 clear_color = ImVec4(0, 0, 0, 0);

    bool done = false;
    while (!done)
    {
        SleepEx(1, false);

        if (GetAsyncKeyState(VK_END))
        {
            if (wegame)
                CleanupUnicorn();

            //clean rendering
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
            CleanupDeviceD3D();
            ::DestroyWindow(hwnd);

            comm->cleanup();
            CloseHandle(comm->deviceHandle);
            exit(0);
        }

        if (GetAsyncKeyState(VK_INSERT))
        {
            show_another_window = !show_another_window;
            SleepEx(150, false);
        }

        // CheckAndReassertTopmost(hwnd);

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (show_another_window)
        {
            ImGuiIO& io = ImGui::GetIO();
            POINT mousePosition;
            GetCursorPos(&mousePosition);
            ScreenToClient(hwnd, &mousePosition);
            io.MousePos.x = mousePosition.x;
            io.MousePos.y = mousePosition.y;
            if (GetAsyncKeyState(VK_LBUTTON))
                io.MouseDown[0] = true;
            else
                io.MouseDown[0] = false;

            DrawMouseDot();

#ifdef NotShxdowBrand
            menu4();
#elif defined(LUKE2)
#ifdef AXADMIN
            menu_red();
#elif defined(GAMELORD)
            menu_purpleblue();
#else
            menu4();
#endif
#elif defined(PANDA)
            menu5();
#elif defined(CHINA)
            menufakehz();
#elif defined(d4ftBrand)
            menu_red();
#else
            drawmenu();
#endif
        }

        renderesp();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        //HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }
}

/*
void CreateConsole()
{
    if (!AllocConsole()) {
        return;
    }

    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    HANDLE hConOut = CreateFileA(("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFileA(("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    CreateConsole();
    */
    
DWORD main() {

    if (!comm) {
        printf("[!] failed to create communication instance\n");
        SleepEx(2000, false);
        return 1;
    }
    //printf("[+] initializing communication channel...\n");

    if (!comm->initialize()) {
        // printf("[!] failed to initialize: %s\n", comm->getLastError());
        printf("[?] Please restart your pc now and run loader again.");
        delete comm;
        SleepEx(3000, false);
        return 1;
    }

    printf("[+] communication channel initialized successfully\n\n");
    
     system("CLS");

    printf("Waiting for the game to start...");
    while (!find_process("DeltaForceClient-Win64-Shipping.exe"))
    {
        SleepEx(250, false);
    }
    system("CLS");

    HWND WeGame = FindWindowW(L"UnrealWindow", L"三角洲行动  ");
    if(WeGame)
    {
        std::string exePath = findProcessPath_NoOpenProcess("DeltaForceClient-Win64-Shipping.exe");

        if (!exePath.empty()) {
            std::cout << "Executable Path: " << exePath << "\n";

            deleteFileTwoUp(exePath, "Config/A.ini");
        }
        else {
            std::cerr << "Process not found.\n";
            SleepEx(2, false);
        }
    }
    system("CLS");

    printf("When in game 'Main Menu' press 'HOME' key\n");
    bool keypressed = false;
    while (!keypressed)
    {
        if (keypressed)
            break;

        if (GetAsyncKeyState(VK_HOME))
            keypressed = true;
    }
    system("CLS");

    processid = find_process("DeltaForceClient-Win64-Shipping.exe");

    if (!processid)
    {
        printf("process not found\n");
        system("PAUSE");
        exit(0);
    }

    GetProcessCr3_2(processid, process_cr3, true);
    printf("Drv->process_cr3:     0x%llx\n", process_cr3);

    bool isbasecorrect = GetBaseAddress(processid, process_base);
    if (!isbasecorrect)
    {
        printf("process base not found\n");
        system("PAUSE");
        exit(0);
    }
    printf("process_base:     0x%llx\n", process_base);

    //dumpprocessexe(Drv->process_base);
    //system("PAUSE");

    get_gamemode();
    getoffsetsfrompatter();

   // ShowWindow(GetConsoleWindow(), SW_HIDE);

    std::thread render_thread(initimguirendershits);
    render_thread.detach();

    if (wegame)
    {
        if (!InitializeUnicorn()) {
            std::cerr << "Failed to initialize Unicorn" << std::endl;
            return 1;
        }
    }

    while (1)
    {
        SleepEx(1500, false);

        std::vector<objInf> entity_list = {};
        std::vector<objItemInf> item_list = {};

        ULONG64 uWorld = read<uintptr_t>(process_base + Offsets::GWorld);
        if (!uWorld) continue;
        //printf("uWorld: %u\n", uWorld);

        ULONG64 uLevels;

        if(wegame)
        {
            uLevels = read<ULONG64>(uWorld + Offsets::PersistentLevel);
            if (!uLevels) continue;
            //printf("uLevels: %u\n", uLevels);

            uLevels = read<ULONG64>(uLevels);
            if (!uLevels) continue;
            //printf("uLevels: %u\n", uLevels);
        }
        else
        {
            uLevels = decrypt_pointer_simple(read<ULONG64>(uWorld + Offsets::PersistentLevel));
            if (!uLevels) continue;
            //printf("uLevels: %u\n", uLevels);
        }

        ULONG32 count = read<ULONG32>(uLevels + Offsets::MaxPacket);
        //printf("count: %i\n", count);

        ULONG64 Actoradd = read<ULONG64>(uLevels + Offsets::OwningActor);
        if (!Actoradd) continue;
        //printf("Actoradd: %u\n", Actoradd);


        uintptr_t LocalPlayer = read<uintptr_t>(Offsets::localplayerscan);
        //printf("LocalPlayer Address: %u\n", LocalPlayer);

        Pointer::PlayerController = read<uintptr_t>(LocalPlayer + Offsets::PlayerController);
        if (Pointer::PlayerController == 0) {
            // std::cerr << "Failed to read PlayerController address!" << std::endl;
            continue;
        }
        //printf("PlayerController Address: %u\n", Pointer::PlayerController);

        Pointer::LocalPawn = decrypt_pointer_simple(read<uintptr_t>(Pointer::PlayerController + Offsets::AcknowledgedPawn));
        if (Pointer::LocalPawn == 0) {
            // std::cerr << "Failed to read LocalPawn address!" << std::endl;
            continue;
        }
        //printf("LocalPawn Address: %u\n", Pointer::LocalPawn);

        uintptr_t LocalPlayerState = read<uintptr_t>(Pointer::LocalPawn + Offsets::PlayerState);
        if (LocalPlayerState == 0) {
            //std::cerr << "Failed to read LocalPlayerState address!" << std::endl;
            //continue;
        }
        //printf("LocalPlayerState Address: 0x%llx\n", LocalPlayerState);

        Pointer::PlayerCameraManager = decrypt_pointer_simple(read<uintptr_t>(Pointer::PlayerController + Offsets::PlayerCameraManager));
        if (Pointer::PlayerCameraManager == 0) {
            //std::cerr << "Failed to read PlayerCameraManager address!" << std::endl;
            continue;
        }
        //printf("PlayerCameraManager Address: %u\n", Pointer::PlayerCameraManager);

        for (ULONG32 i = 0; i < count; i++) {
            ULONG64 Actor = read<ULONG64>(Actoradd + i * 8);
            if (!Actor) continue;
            //printf("actor: %u\n", Actor);

            objInf currentObj;

            uintptr_t playerstate = read<uintptr_t>(Actor + Offsets::PlayerState);
            //printf("PlayerState Address: 0x%llx\n", playerstate);

           // if (!playerstate && !settings::showbots) continue;

           // bool isDead = read<bool>(playerstate + Offsets::bDead);
            bool isBot = read<bool>(playerstate + Offsets::bIsABot);
            //printf("isBot: %i\n", isBot);

            if (!isBot && !settings::showbots) continue;
            //if (!isDead) continue;

            int localteam = read<int>(LocalPlayerState + Offsets::Camp);
            int localcamp = read<int>(LocalPlayerState + Offsets::TeamId);
            int enemyteam = read<int>(playerstate + Offsets::Camp);
            int enemycamp = read<int>(playerstate + Offsets::TeamId);

            if (localteam == enemyteam && settings::teamcheck_war)
                continue;
            if (localcamp == enemycamp && settings::teamcheck_op)
                continue;

            ImColor TeamColor = GetTeamColor(enemycamp);

            auto playerpawn = read<uintptr_t>(Actor + Offsets::Instigator);
            if (playerpawn == Pointer::LocalPawn || !playerpawn) continue; // Localplayer Check
            //printf("playerpawn Address: 0x%llx\n", playerpawn);

            auto Mesh = read<uintptr_t>(playerpawn + Offsets::Mesh);
            if (!Mesh) continue;
            //printf("Mesh Address: 0x%llx\n", Mesh);

            uintptr_t BoneArray = read<uintptr_t>(Mesh + Offsets::StaticMesh);
            if (!is_valid(BoneArray))
                BoneArray = read<uintptr_t>(Mesh + Offsets::StaticMesh + 0x10);

            //printf("BoneArray Address: 0x%llx\n", BoneArray);


            std::wstring wwa = ReadFStringW(playerstate + Offsets::PlayerNamePrivate);
            std::string playername = WideToUTF8(wwa);

            auto tagArray = read<TArray<EAICharacterTag>>(Actor + Offsets::CharacterTags);
            EAICharacterTag Tag = EAICharacterTag::EAICharacterTag_None;

            if (!isBot) {
                if (tagArray.Count > 0 && tagArray.Count < 100) {
                    EAICharacterTag tagBuffer[100]{};
                    ReadMemory((PVOID)tagArray.Data, &tagBuffer, tagArray.Count);

                    for (int j = 0; j < tagArray.Count; j++) {
                        if (tagBuffer[j] != EAICharacterTag::EAICharacterTag_None) {
                            Tag = tagBuffer[j];
                            break;
                        }
                    }
                }

                playername = GetTagString(Tag);
            }

            currentObj.entityaddy = Actor;
            currentObj.Mesh = Mesh;
            currentObj.playerpawn = playerpawn;
            currentObj.playerstate = playerstate;
            currentObj.BoneArray = BoneArray;
            currentObj.isBot = isBot;
            currentObj.playername = playername;
            currentObj.TeamColor = TeamColor;

            entity_list.push_back(currentObj);

        }
        {
            std::lock_guard<std::mutex> lock(cacheMtx);
            get_entities = std::move(entity_list);
        }
        if(!wegame)
        {
            for (int i = 0; i < count; ++i) {
                objItemInf currentObj;

                uintptr_t Actor = read<uintptr_t>(Actoradd + i * 8);

                if (Actor)
                {
                    //printf("Actor %d: 0x%llx\n", i, Actor);

                    uint32_t ObjectID = read<uint32_t>(Actor + Offsets::ObjectIDName);
                    //printf("ObjectID: %u\n", ObjectID);
                    if (!ObjectID)continue;

                    std::string name = get_fname(ObjectID);
                    //printf("name; %s\n", name.c_str());

                    if (name.find("None") != std::string::npos)
                        continue;

                    // printf("Name: %s\n", name.c_str());

                    EMarkingItemType MarkingItemType = read<EMarkingItemType>(Actor + Offsets::MarkingItemType);

                    //if (ToString(currentObj.MarkingItemType).find("None") != std::string::npos)
                        //continue;
                    //if (ToString(currentObj.MarkingItemType).find("Unknown") != std::string::npos)
                        //continue;

                    currentObj.entityaddy = Actor;
                    currentObj.ClassName = name;
                    currentObj.MarkingItemType = MarkingItemType;
                    item_list.push_back(currentObj);
                }
            }

            {
                std::lock_guard<std::mutex> lock(cacheMtx2);
                get_items = std::move(item_list); // more efficient
            }
        }
    }

    if (wegame)
        CleanupUnicorn();

    comm->cleanup();
    CloseHandle(comm->deviceHandle);
}

/*
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        if (GetModuleHandle(NULL))
        {
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        }
    }
    }

    return TRUE;
}
*/