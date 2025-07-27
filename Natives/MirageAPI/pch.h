#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// commenting libs
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

/* ---Includes--- */
#include <msclr/marshal_cppstd.h>  // CLR
// native need
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

// window enums
#include "Window/NativeWindowEnums.h"

// event enums
#include "Events/NativeEvents.h"

// DX12 enums
#include "DerectX/DX12Enums.h"
// and misk
#include "DerectX/DX12Helpers.h"
#include "DerectX/DX12Context.h"

#endif
