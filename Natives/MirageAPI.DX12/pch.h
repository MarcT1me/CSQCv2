#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// defines
#define CheckMissmatch(first, second) if ((first) != (second))
#define CheckNull(resource) if (!(resource))

#define SimpleDelete(resource) if ((resource)) { delete (resource); (resource) = nullptr; }
#define SimpleDeleteArr(resource) if ((resource)) { delete[] (resource); (resource) = nullptr; }
#define SimpleRelease(resource) if ((resource)) { (resource)->Release(); (resource) = nullptr; }

// includes
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>

// other
#include "DX12Device.h"
#include "DX12Exception.h"

#endif
