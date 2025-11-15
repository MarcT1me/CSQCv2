#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#define XR_USE_PLATFORM_WIN64
#define XR_USE_GRAPHICS_API_D3D12

// commenting libs
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dwmapi.lib")

/* ---Includes--- */
#include <msclr/marshal_cppstd.h>  // CLR
// native need
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

using namespace System;
using namespace OpenTK::Mathematics;

#include "Helpers.h"
#include "Exceptions.h"
#include "rect.h"

// Window and window tools
#include "WinTools/Window/enums.h"
#include "WinTools/Events/Events.h"
#include "WinTools/Cursor/enums.h"
#include "WinTools/Icon/enums.h"
#include "WinTools/Menu/enums.h"

// DX12
#include "DerectX/enums.h"
#include "DerectX/DX12Exceptions.h"
#include "DerectX/DX12Device.h"
#include "DerectX/DX12Helpers.h"
#include "DerectX/Command/enums.h"
#include "DerectX/Pipeline/enums.h"
#include "DerectX/Resource/enums.h"
#include "DerectX/Shader/enums.h"
#include "DerectX/DX12Object.h"


#endif
