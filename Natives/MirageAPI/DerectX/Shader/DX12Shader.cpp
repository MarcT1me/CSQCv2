#include "pch.h"
#include "DX12Shader.h"

#include <cstdio>
#include <vcclr.h>

namespace MirageAPI::DirectX::Shader
{
    DX12Shader::DX12Shader(
        ID3DBlob* bytecode,
        DX12ShaderType type
    ) : DX12ObjectData(nullptr),
        _bytecode(bytecode),
        _type(type)
    {
        if (bytecode)
            bytecode->AddRef();
    }

    DX12Shader::DX12Shader(
        array<Byte>^ byteArray,
        DX12ShaderType type
    ) : DX12ObjectData(nullptr),
        _bytecode(nullptr),
        _type(type)
    {
        pin_ptr<Byte> pinnedData = &byteArray[0];
        UINT dataSize = byteArray->Length;

        ID3DBlob* bytecode;
        CheckHResult(
            D3DCreateBlob(dataSize, &bytecode),
            "Failed to create shader blob from byte array"
        );
        _bytecode = bytecode;

        memcpy(_bytecode->GetBufferPointer(), pinnedData, dataSize);
    }

    DX12Shader::!DX12Shader()
    {
        SimpleRelease(_bytecode);
    }

    D3D12_SHADER_BYTECODE DX12Shader::NativeBytecode::get()
    {
        if (!_bytecode)
        {
            return {nullptr, 0};
        }

        return {
            _bytecode->GetBufferPointer(),
            _bytecode->GetBufferSize()
        };
    }

    void DX12Shader::SaveToFile(String^ filePath)
    {
        if (!_bytecode) return;

        pin_ptr<const wchar_t> nativePath = PtrToStringChars(filePath);
        std::FILE* file;

        if (_wfopen_s(&file, nativePath, L"wb") != 0)
        {
            throw gcnew DXException("Failed to open file for writing");
        }

        fwrite(_bytecode->GetBufferPointer(), 1, _bytecode->GetBufferSize(), file);
        fclose(file);
    }

    DX12Shader^ DX12Shader::LoadFromFile(String^ filePath, DX12ShaderType type)
    {
        pin_ptr<const wchar_t> nativePath = PtrToStringChars(filePath);
        FILE* file;

        if (_wfopen_s(&file, nativePath, L"rb") != 0)
        {
            throw gcnew DXException("Shader file not found");
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        array<Byte>^ byteArray = gcnew array<Byte>(fileSize);
        pin_ptr<Byte> pinnedArray = &byteArray[0];

        fread(pinnedArray, 1, fileSize, file);
        fclose(file);

        return gcnew DX12Shader(byteArray, type);
    }
}
