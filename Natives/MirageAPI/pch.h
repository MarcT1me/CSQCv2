#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#pragma comment(lib, "d3d12.lib")
#include <d3d12.h>

#pragma comment(lib, "dxgi.lib")
#include <dxgi1_4.h>

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

#include "Window/NativeWindowEnums.h"

#include "Events/NativeEvents.h"

#include "DerectX/DX12Enums.h"
#include "DerectX/DX12Context.h"
#include "DerectX/extensions.h"

#endif
