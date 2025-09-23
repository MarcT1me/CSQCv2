#pragma once

#include "DX12ContextConfig.h"

#include "DX12SwapChain.h"
#include "../Command/DX12CommandQueue.h"
#include "../Command/DX12CommandList.h"
#include "../Command/DX12Fence.h"
#include "../Shader/DX12ShaderProgram.h"

namespace MirageAPI::DirectX
{
    public ref class DX12Context : public DX12Object<DX12ContextConfig^>
    {
        Command::DX12Fence^ _fence;
        Command::DX12CommandQueue^ _commandQueue;
        Command::DX12CommandList^ _commandList;
        DX12SwapChain^ _swapChain;

        Rect^ m_winRect;
        bool m_isResized;

        void HandleResize();

    public:
        Shader::DX12ShaderProgram^ ShaderProgram;

        // constructors and deconstructors
        DX12Context(
            DX12ContextConfig^ config,
            HWND hwnd
        );

        ~DX12Context() { this->!DX12Context(); }
        !DX12Context();

        // native
        property Command::DX12CommandQueue^ CmdQueue
        {
            Command::DX12CommandQueue^ get() { return _commandQueue; }
        }
        property Command::DX12CommandList^ CmdList
        {
            Command::DX12CommandList^ get() { return _commandList; }
        }
        property DX12SwapChain^ SwapChain
        {
            DX12SwapChain^ get() { return _swapChain; }
        }
        // config
        property UINT VSync
        {
            UINT get() { return MetaData->VSyncInterval; }
            void set(UINT value) { MetaData->VSyncInterval = value; }
        }
        property Vector2i Resolution
        {
            Vector2i get() { return MetaData->Resolution; }
        }
        // other
        property bool IncorrectSize
        {
            bool get() { return MetaData->Resolution.X <= 0 || MetaData->Resolution.Y <= 0; }
        }

        // context methods
        void SetResolution(Vector2i^ resolution);
        void SetWinRect(Rect^ winRect);
        void SetViewport(Rect^ viewportRect);
        void SetClipPlanes(Vector2^ depth);
        
        void BeginFrame();
        void Clear(Color4 color);
        void EndFrame();
        void Present();
    };
}
