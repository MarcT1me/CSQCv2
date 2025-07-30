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
        property UINT ResolutionX
        {
            UINT get() { return m_config->ResolutionX; }
        }
        property UINT ResolutionY
        {
            UINT get() { return m_config->ResolutionY; }
        }
        // other
        property bool IncorrectSize
        {
            bool get()
            {
                return ResolutionX <= 0 || ResolutionY <= 0 ||
                    m_config->Viewport->Width <= 0 || m_config->Viewport->Height <= 0;
            }
        }

        // context methods
        void Resize(UINT width, UINT height);
        void SetViewport(float x, float y, float width, float height);
        void SetViewportDepth(float x, float y);
        void Clear(float r, float g, float b, float a);
        void BeginFrame();
        void EndFrame();
        void Present();
    };
}
