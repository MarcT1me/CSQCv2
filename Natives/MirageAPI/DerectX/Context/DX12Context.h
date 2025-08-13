#pragma once

#include "DX12ContextConfig.h"

#include "DX12SwapChain.h"
#include "../Command/DX12CommandQueue.h"
#include "../Command/DX12CommandList.h"
#include "../Command/DX12Fence.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Context : public DX12Object
    {
        // swap chain
        DX12SwapChain^ m_swapChain;
        // cmd
        Command::DX12Fence^ m_fence;
        Command::DX12CommandQueue^ m_commandQueue;
        Command::DX12CommandList^ m_commandList;
        // config
        DX12ContextConfig^ m_config;

        DoubleRect^ m_winRect;
        bool m_isResized;

        void HandleResize();

    public:
        // constructors and deconstructors
        DX12Context(
            HWND hwnd,
            DX12ContextConfig^ config
        );

        ~DX12Context() { this->!DX12Context(); }
        !DX12Context();

        // native
        property Command::DX12CommandList^ CmdList
        {
            Command::DX12CommandList^ get() { return m_commandList; }
        }
        property Command::DX12CommandQueue^ CmdQueue
        {
            Command::DX12CommandQueue^ get() { return m_commandQueue; }
        }
        // config
        property UINT VSync
        {
            UINT get() { return m_config->VSyncInterval; }
            void set(UINT value) { m_config->VSyncInterval = value; }
        }
        property Vector2i Resolution
        {
            Vector2i get() { return m_config->Resolution; }
        }
        // other
        property bool IncorrectSize
        {
            bool get() { return m_config->Resolution.X <= 0 || m_config->Resolution.Y <= 0; }
        }

        // context methods
        void SetResolution(Vector2i^ resolution);
        void SetWinRect(DoubleRect^ winRect);
        void SetViewport(SimpleRect^ viewportRect);
        void SetClipPlanes(Vector2^ depth);
        void BeginFrame();
        void Clear(Color4 color);
        void EndFrame();
        void Present();
    };
}
