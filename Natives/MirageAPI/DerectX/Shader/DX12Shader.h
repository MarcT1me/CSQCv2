#pragma once

#include "../DX12ObjectData.h"

namespace MirageAPI::DirectX::Shader
{
    public ref class DX12Shader : DX12ObjectData
    {
        ID3DBlob* _bytecode;
        DX12ShaderType _type;

    public:
        DX12Shader(ID3DBlob* bytecode, DX12ShaderType type);
        DX12Shader(array<Byte>^ byteArray, DX12ShaderType type);

        ~DX12Shader() { this->!DX12Shader(); }
        !DX12Shader();

        property ID3DBlob* Bytecode
        {
            ID3DBlob* get() { return _bytecode; }
        }
        property DX12ShaderType Type
        {
            DX12ShaderType get() { return _type; }
        }
        property D3D12_SHADER_BYTECODE NativeBytecode
        {
            D3D12_SHADER_BYTECODE get();
        }

        void SaveToFile(String^ filePath);
        static DX12Shader^ LoadFromFile(String^ filePath, DX12ShaderType type);
    };
}
