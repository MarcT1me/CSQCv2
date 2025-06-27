#include "pch.h"
#include "DX12Helpers.h"

namespace MirageAPI::DirectX
{
    UINT64 GetRequiredIntermediateSize(
        ID3D12Resource* pDestinationResource,
        UINT FirstSubresource,
        UINT NumSubresources
    )
    {
        D3D12_RESOURCE_DESC Desc = pDestinationResource->GetDesc();
        UINT64 RequiredSize = 0;
        ID3D12Device* pDevice;
        pDestinationResource->GetDevice(__uuidof(ID3D12Device), (void**)&pDevice);
        pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr,
                                       &RequiredSize);
        pDevice->Release();
        return RequiredSize;
    }

    void UpdateSubresources(
        ID3D12GraphicsCommandList* pCmdList,
        ID3D12Resource* pDestinationResource,
        ID3D12Resource* pIntermediate,
        UINT64 IntermediateOffset,
        UINT FirstSubresource,
        UINT NumSubresources,
        D3D12_SUBRESOURCE_DATA* pSrcData)
    {
        // Упрощенная реализация для одного субресурса
        if (NumSubresources == 1)
        {
            D3D12_RESOURCE_DESC desc = pDestinationResource->GetDesc();
            D3D12_TEXTURE_COPY_LOCATION dst = {
                pDestinationResource,
                D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX,
                FirstSubresource
            };
            D3D12_TEXTURE_COPY_LOCATION src = {
                pIntermediate,
                D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
                {
                    IntermediateOffset, {
                        desc.Format,
                        static_cast<UINT>(desc.Width),
                        static_cast<UINT>(desc.Height),
                        static_cast<UINT>(desc.DepthOrArraySize),
                        static_cast<UINT>(pSrcData->RowPitch)
                    }
                }
            };
            pCmdList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        }
        else
        {
            // Для нескольких субресурсов потребуется более сложная реализация
            throw gcnew System::NotImplementedException("Multi-subresource updates not implemented");
        }
    }
}
