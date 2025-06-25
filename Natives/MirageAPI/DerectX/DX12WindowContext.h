#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

namespace MirageAPI::DirectX
{
    public ref class DX12WindowContext
    {
    internal:
        IDXGISwapChain3* m_swapChain = nullptr;
        ID3D12Resource* m_renderTarget0;
        ID3D12Resource* m_renderTarget1;
        ID3D12CommandAllocator* m_commandAllocator = nullptr;
        ID3D12GraphicsCommandList* m_commandList = nullptr;

        // Для синхронизации
        ID3D12Fence* m_fence;
        UINT64 m_fenceValue;
        HANDLE m_fenceEvent;

        // Дескрипторы
        ID3D12DescriptorHeap* m_rtvHeap = nullptr;
        D3D12_CPU_DESCRIPTOR_HANDLE* m_rtvHandle0;
        D3D12_CPU_DESCRIPTOR_HANDLE* m_rtvHandle1;
        UINT m_rtvDescriptorSize;

        int m_frameIndex = 0;
        int m_width;
        int m_height;
        int m_vsync = 0;

    public:
        DX12WindowContext(HWND hwnd, int width, int height);
        ~DX12WindowContext();

        void Cleanup();

        void Resize(int width, int height);
        void BeginFrame();
        void EndFrame();
        void Present();
        void Clear(float r, float g, float b, float a);

        property int VSync
        {
            int get() { return m_vsync; }
            void set(int value) { m_vsync = value; }
        }
    };
}
