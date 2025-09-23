#include "pch.h"
#include "DX12SwapChain.h"

#include "../Command/DX12CommandList.h"
#include "../Descriptors/DX12DescriptorHeap.h"
#include "../Command/DX12CommandQueue.h"
#include "../Resource/ShaderResources/DX12RenderTarget.h"

namespace MirageAPI::DirectX
{
    DX12SwapChain::DX12SwapChain(
        DX12ContextConfig^ config,
        HWND hwnd,
        Command::DX12CommandQueue^ commandQueue
    ) : DX12Object(config)
    {
        // needed in swap chain initializations
        auto factory = DX12Device::GetNativeFactory(true);

        DXGI_SWAP_CHAIN_DESC1 desc = config->NativeSwapChainDesk();
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
        DXGI_SWAP_CHAIN_FULLSCREEN_DESC* fullscreenDescPtr = nullptr;

        if (config->FullscreenConfig)
        {
            fullscreenDesc = config->FullscreenConfig->NativeDesk();
            fullscreenDescPtr = &fullscreenDesc;
        }

        IDXGISwapChain1* hwndSwapChain;
        CheckHResult(
            factory->CreateSwapChainForHwnd(
                commandQueue->NativeQueue,
                hwnd,
                &desc,
                fullscreenDescPtr,
                nullptr,
                &hwndSwapChain
            ),
            "Create SwapChain failed"
        );

        IDXGISwapChain3* swapChain;
        CheckHResult(
            hwndSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain)),
            "QueryInterface for SwapChain failed"
        );
        hwndSwapChain->Release();
        _swapChain = swapChain;

        UpdateFrameIndex();

        CreateBuffers();

        CheckHResult(DX12Device::GetDeviceRemovedReason(), "Error");
    }

    void DX12SwapChain::!DX12SwapChain()
    {
        Validate();

        FreeBuffers();

        SimpleRelease(_swapChain);
    }

    void DX12SwapChain::CreateBuffers()
    {
        _rtHeap = gcnew Descriptors::DX12DescriptorHeap(
            gcnew Descriptors::DX12DescriptorHeapData(
                MetaData->Identifier->ChangeName("&ID (RTHeap)"),
                Descriptors::DX12DescriptorHeapType::RTV,
                MetaData->BufferCount,
                false, false
            )
        );

        // creating frame buffer array
        _frameBuffers = gcnew array<Resource::DX12RenderTarget^>(MetaData->BufferCount);

        // and buffers
        for (UINT i = 0; i < MetaData->BufferCount; i++)
        {
            // creating resource
            ID3D12Resource* renderTarget = nullptr;
            CheckHResult(
                _swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTarget)),
                "GetBuffer failed"
            );

            // creating buffer himself
            Resource::DX12RenderTarget^ buffer = gcnew Resource::DX12RenderTarget(
                Resource::DX12ResourceConfig::RenderTargetConfig(
                    MetaData->Identifier->ChangeName("&ID (RT:" + i + ")"),
                    static_cast<UINT>(MetaData->Resolution.X),
                    static_cast<UINT>(MetaData->Resolution.Y),
                    MetaData->Format,
                    Resource::DX12ResourceFlags::None
                ),
                renderTarget
            );
            // render target view for buffer
            if (!buffer->AllocateResource(_rtHeap))
            {
                throw gcnew DXException("Failed to allocate frame buffer: " + i);
            }
            // save him
            _frameBuffers[i] = buffer;
        }
    }

    void DX12SwapChain::FreeBuffers()
    {
        if (_frameBuffers)
        {
            for each (Resource::DX12RenderTarget^ frameBuffer in _frameBuffers)
            {
                if (frameBuffer != nullptr)
                    delete frameBuffer;
            }
            _frameBuffers = nullptr;
        }

        SimpleDelete(_rtHeap);
    }

    void DX12SwapChain::UpdateBufferSizes()
    {
        FreeBuffers();

        DXGI_SWAP_CHAIN_DESC desc;
        _swapChain->GetDesc(&desc);

        CheckHResult(
            _swapChain->ResizeBuffers(
                MetaData->BufferCount,
                static_cast<UINT>(MetaData->Resolution.X),
                static_cast<UINT>(MetaData->Resolution.Y),
                desc.BufferDesc.Format,
                desc.Flags
            ),
            "Swap chain resize failed"
        );

        UpdateFrameIndex();

        CreateBuffers();
    }

    void DX12SwapChain::UpdateFrameIndex()
    {
        _frameIndex = _swapChain->GetCurrentBackBufferIndex();
    }

    Resource::DX12RenderTarget^ DX12SwapChain::AcquireNextBackBuffer(Command::DX12CommandList^ commandList)
    {
        _currentFrameBuffer =
            _frameBuffers != nullptr && _frameIndex < static_cast<UINT>(_frameBuffers->Length)
                ? _frameBuffers[_frameIndex]
                : throw gcnew DXException("Cant acquire next frame buffer");
        _currentFrameBuffer->TransitionState(commandList, Resource::DX12ResourceState::RenderTarget);
        return _currentFrameBuffer;
    }

    void DX12SwapChain::ReleaseBackBufferToPresent(Command::DX12CommandList^ commandList)
    {
        _currentFrameBuffer->TransitionState(commandList, Resource::DX12ResourceState::Present);
    }

    void DX12SwapChain::Present()
    {
        CheckHResult(
            _swapChain->Present(MetaData->VSyncInterval, 0),
            "present swap chain"
        );
        UpdateFrameIndex();
    }
}
