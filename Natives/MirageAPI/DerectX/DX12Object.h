#pragma once

namespace MirageAPI::DirectX
{
    public ref class DX12Object abstract
    {
    protected:
        bool disposed = false;
        ID3D12Device* device;

    public:
        // constructors and deconstructors
        DX12Object()
        {
            device = DX12Device::GetNativeDevice(true);
        }

        virtual ~DX12Object()
        {
            disposed = true;
        }

        // other getters
        property bool Disposed
        {
            bool get() { return disposed; }
        }

        // other methods
        virtual void Validate()
        {
            if (disposed)
            {
                throw gcnew InvalidOperationException("Can't operate on disposed object");
            }
        }
    };
}
