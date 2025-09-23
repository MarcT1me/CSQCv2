#pragma once

#include "../DX12ObjectData.h"

namespace MirageAPI::DirectX::Descriptors
{
    public ref class DX12DescriptorHeapData : public DX12ObjectData
    {
    internal:
        // description
        DX12DescriptorHeapType _type;
        UINT _capacity;
        bool _shaderVisible;
        UINT _descriptorSize;

    public:
        bool AutoAllocate;

        DX12DescriptorHeapData(
            QIdentifier^ identifier,
            DX12DescriptorHeapType heapType,
            UINT capacity,
            bool shaderVisible,
            bool autoAllocate
        ) : DX12ObjectData(identifier),
            _type(heapType),
            _capacity(capacity),
            _shaderVisible(shaderVisible),
            AutoAllocate(autoAllocate)
        {
        }

        property DX12DescriptorHeapType Type
        {
            DX12DescriptorHeapType get() { return _type; }
        }
        property UINT Capacity
        {
            UINT get() { return _capacity; }
        }
        property UINT DescriptorSize
        {
            UINT get() { return _descriptorSize; }
        }
        property bool IsShaderVisible
        {
            bool get() { return _shaderVisible; }
        }

        void Validate()
        {
            ThrowIfNull(
                _descriptorSize,
                CSFormat(
                    "Invalid descriptor size. Capacity: {0}, DescriptorSize: {1}",
                    _capacity, _descriptorSize
                )
            );
            ThrowIfNull(
                _capacity,
                CSFormat(
                    "Invalid heap capacity. Capacity: {0}, DescriptorSize: {1}",
                    _capacity, _descriptorSize
                )
            );
        }

        String^ ToString() override
        {
            return CSFormat(
                "DX12DescriptorHeapData{0}(type: {1}, capacity: {2}, descriptor size: {3}, visible: {4})",
                Identifier, _type, _capacity, _descriptorSize, _shaderVisible
            );
        }
    };
}
