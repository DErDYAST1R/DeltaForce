#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <dwmapi.h>
#include <random>
#include <D3DX11tex.h>
#include <d3dx10math.h>
#include <d3d11.h>
#include <tchar.h>

#include "BRAND.h"
#ifdef NotShxdowBrand
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/notshxdow/imgui_1.h"
#include "ImGui/notshxdow/imgui_impl_win32_1.h"
#include "ImGui/notshxdow/imgui_impl_dx11_1.h"
#include "ImGui/ui/ui.hpp"
#else
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/custom_imgui.h"
#include "ImGui/bytes.h"
#endif

#include "offsets.h"
#include "driver.h"
#include "decryption.h"


#include "math2.h"
#include "renderstuff.h"
#include "sdk.h"

