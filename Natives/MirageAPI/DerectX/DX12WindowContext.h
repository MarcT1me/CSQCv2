#pragma once

#include "DX12ContextConfig.h"

#include "DX12Object.h"
#include "CommandList/DX12CommandQueue.h"
#include "CommandList/DX12WindowCommandList.h"
#include "Resource/Texture/DX12FrameBuffer.h"

namespace MirageAPI::DirectX
{
    public ref class DX12WindowContext : public DX12Object
    {
    internal:
        IDXGISwapChain3* m_swapChain = nullptr;

        DX12Fence^ m_fence = nullptr;
        DX12DescriptorHeap^ m_rtvHeap;
        
        array<Resource::DX12FrameBuffer^>^ m_frameBuffers;
        UINT m_frameIndex = 0;
        UINT m_bufferCount = 2;

        CommandList::DX12CommandQueue^ m_commandQueue;
        CommandList::DX12WindowCommandList^ m_windowCommandList;

        int m_width = 0;
        int m_height = 0;
        int m_vsync = 0;
        DX12WindowContextConfig^ m_config;

        void CreateFrameBuffers();
        
        void UpdateFrameIndex();
        
        void FreeFrameBuffers();
        void FreeRTVHeap();

    public:
        DX12WindowContext(
            HWND hwnd,
            int width, int height,
            DX12WindowContextConfig^ config
        );

        ~DX12WindowContext() { this->!DX12WindowContext(); }
        !DX12WindowContext();

        void Resize(int width, int height);
        void SetViewport(float x, float y, float width, float height);
        void SetViewportDepth(float x, float y);
        void Clear(float r, float g, float b, float a);
        void BeginFrame();
        void EndFrame();
        void Present();

        property Resource::DX12FrameBuffer^ CurrentFrameBuffer
        {
            Resource::DX12FrameBuffer^ get()
            {
                return m_frameBuffers != nullptr && m_frameIndex < static_cast<UINT>(m_frameBuffers->Length)
                           ? m_frameBuffers[m_frameIndex]
                           : nullptr;
            }
        }

        property CommandList::DX12WindowCommandList^ CmdList { CommandList::DX12WindowCommandList^ get() { return m_windowCommandList; } }
        property DX12DescriptorHeap^ RTVHeap { DX12DescriptorHeap^ get() { return m_rtvHeap; } }

        property int VSync
        {
            int get() { return m_vsync; }
            void set(int value) { m_vsync = value; }
        }
    };
}
