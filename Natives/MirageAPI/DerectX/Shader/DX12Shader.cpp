#include "pch.h"
#include "DX12Shader.h"

#include <cstdio>
#include <vcclr.h>

namespace MirageAPI::DirectX::Shader
{
    DX12Shader::DX12Shader(
        ID3DBlob* bytecode,
        DX12ShaderType type
    ) : m_bytecode(bytecode),
        m_type(type)
    {
        if (bytecode)
            bytecode->AddRef();
    }

    DX12Shader::DX12Shader(
        array<System::Byte>^ byteArray,
        DX12ShaderType type
    ) : m_bytecode(nullptr),
        m_type(type)
    {
        pin_ptr<System::Byte> pinnedData = &byteArray[0];
        UINT dataSize = byteArray->Length;

        ID3DBlob* bytecode;
        HRESULT hr = D3DCreateBlob(dataSize, &bytecode);
        if (FAILED(hr))
        {
            throw gcnew System::Exception("Failed to create shader blob from byte array");
        }
        m_bytecode = bytecode;

        memcpy(m_bytecode->GetBufferPointer(), pinnedData, dataSize);
    }

    DX12Shader::!DX12Shader()
    {
        SimpleRelease(m_bytecode);
    }

    D3D12_SHADER_BYTECODE DX12Shader::NativeBytecode::get()
    {
        if (!m_bytecode)
        {
            return {nullptr, 0};
        }

        return {
            m_bytecode->GetBufferPointer(),
            m_bytecode->GetBufferSize()
        };
    }

    void DX12Shader::SaveToFile(System::String^ filePath)
    {
        if (!m_bytecode) return;

        pin_ptr<const wchar_t> nativePath = PtrToStringChars(filePath);
        std::FILE* file;

        if (_wfopen_s(&file, nativePath, L"wb") != 0)
        {
            throw gcnew System::Exception("Failed to open file for writing");
        }

        fwrite(m_bytecode->GetBufferPointer(), 1, m_bytecode->GetBufferSize(), file);
        fclose(file);
    }

    DX12Shader^ DX12Shader::LoadFromFile(System::String^ filePath, DX12ShaderType type)
    {
        pin_ptr<const wchar_t> nativePath = PtrToStringChars(filePath);
        FILE* file;

        if (_wfopen_s(&file, nativePath, L"rb") != 0)
        {
            throw gcnew System::Exception("Shader file not found");
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        array<System::Byte>^ byteArray = gcnew array<System::Byte>(fileSize);
        pin_ptr<System::Byte> pinnedArray = &byteArray[0];

        fread(pinnedArray, 1, fileSize, file);
        fclose(file);

        return gcnew DX12Shader(byteArray, type);
    }
}
