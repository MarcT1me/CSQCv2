#include "pch.h"
#include "DX12Context.h"

#include "../Descriptors/DX12DescriptorHeap.h"

namespace MirageAPI::DirectX
{
    DX12Context::DX12Context(
        DX12ContextConfig^ config,
        HWND hwnd
    ) : DX12Object(config)
    {
        _fence = gcnew Command::DX12Fence(config->Identifier, 1);
        _commandQueue = gcnew Command::DX12CommandQueue(config->Identifier, Command::DX12CommandListType::Direct, _fence);
        _commandList = gcnew Command::DX12CommandList(config->Identifier, _commandQueue);
        _swapChain = gcnew DX12SwapChain(config, hwnd, _commandQueue);
    }

    DX12Context::!DX12Context()
    {
        Validate();

        if (_commandQueue)
        {
            _commandQueue->Signal();
            _commandQueue->Wait();
        }
        if (_fence)
        {
            _fence->Wait();
        }

        SimpleDelete(_fence);
        SimpleDelete(_commandList);
        SimpleDelete(_commandQueue);
    }

    void DX12Context::SetWinRect(Rect^ winRect)
    {
        m_winRect = winRect;
    }

    void DX12Context::SetResolution(Vector2i^ resolution)
    {
        MetaData->Resolution = *resolution;
        m_isResized = true;
    }

    void DX12Context::HandleResize()
    {
        // wait last frame
        _commandQueue->Wait();
        _fence->Wait();

        // update swap chain buffers
        _swapChain->UpdateBufferSizes();
        m_isResized = false;
    }

    void DX12Context::SetViewport(Rect^ viewportRect)
    {
        MetaData->Viewport = viewportRect;
    }

    void DX12Context::SetClipPlanes(Vector2^ depth)
    {
        MetaData->Near = depth->X;
        MetaData->Far = depth->Y;
    }

    void DX12Context::BeginFrame()
    {
        if (IncorrectSize) return;

        if (m_isResized) HandleResize();

        // prepare queue
        _commandQueue->Signal();

        // prepare command list
        ShaderProgram->BeginFrame(
            _swapChain->RTHeap->GetResourceDescriptor(
                reinterpret_cast<Resource::DX12ShaderResource^>(_swapChain->AcquireNextBackBuffer(_commandList))
            ),
            _commandList
        );

        // setting render field
        _commandList->SetViewport(
            static_cast<float>(MetaData->Viewport->X),
            static_cast<float>(MetaData->Viewport->Y),
            static_cast<float>(MetaData->Viewport->Width),
            static_cast<float>(MetaData->Viewport->Height),
            static_cast<float>(MetaData->Near),
            static_cast<float>(MetaData->Far)
        );
        _commandList->SetScissorRect(
            MetaData->Viewport->X,
            MetaData->Viewport->Y,
            MetaData->Viewport->Width,
            MetaData->Viewport->Height
        );
    }

    void DX12Context::Clear(Color4 color)
    {
        if (IncorrectSize) return;

        ShaderProgram->Clear(color);
    }

    void DX12Context::EndFrame()
    {
        if (IncorrectSize) return;

        // go into present mode
        ShaderProgram->ReleaseFrame();
        _swapChain->ReleaseBackBufferToPresent(_commandList);

        // execute
        _commandQueue->ExecuteList(_commandList);
        _commandQueue->Wait();
    }

    void DX12Context::Present()
    {
        if (IncorrectSize) return;

        _swapChain->Present();
        _fence->Wait();
    }
}
