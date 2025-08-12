#include "pch.h"
#include "DX12ShaderCompiler.h"

#include <ostream>
#include <comdef.h>

namespace MirageAPI::DirectX::Shader
{
    DX12Shader^ DX12ShaderCompiler::CompileShaderFromFile(
        const wchar_t* filename,
        DX12ShaderType shaderType,
        const char* entry,
        const char* target,
        DX12ShaderCompileFlags compileFlags0,
        DX12ShaderCompileFlags compileFlags1,
        const D3D_SHADER_MACRO* defines
    )
    {
        ID3DBlob* bytecode = nullptr;
        ID3DBlob* errors = nullptr;

        HRESULT hr = D3DCompileFromFile(
            filename,
            defines,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entry,
            target,
            static_cast<UINT>(compileFlags0),
            static_cast<UINT>(compileFlags1),
            &bytecode,
            &errors
        );

        if (FAILED(hr))
        {
            if (errors)
            {
                const char* errorMsg = static_cast<const char*>(errors->GetBufferPointer());
                String^ managedError = gcnew String(errorMsg);
                errors->Release();
                throw gcnew DXException("Failed to compile shader: " + managedError);
            }
            _com_error err(hr);
            LPCTSTR errMsg = err.ErrorMessage();
            String^ managedError = gcnew String(errMsg);
            throw gcnew DXException("Failed to compile shader: " + managedError);
        }

        return gcnew DX12Shader(bytecode, shaderType);
    }

    DX12Shader^ DX12ShaderCompiler::CompileShaderFromSource(
        const std::string& source,
        DX12ShaderType shaderType,
        const char* entry,
        const char* target,
        DX12ShaderCompileFlags compileFlags0,
        DX12ShaderCompileFlags compileFlags1,
        const D3D_SHADER_MACRO* defines
    )
    {
        ID3DBlob* bytecode = nullptr;
        ID3DBlob* errors = nullptr;

        HRESULT hr = D3DCompile(
            source.c_str(),
            source.length(),
            nullptr,
            defines,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entry,
            target,
            static_cast<UINT>(compileFlags0),
            static_cast<UINT>(compileFlags1),
            &bytecode,
            &errors
        );

        if (FAILED(hr))
        {
            if (errors)
            {
                const char* errorMsg = static_cast<const char*>(errors->GetBufferPointer());
                String^ managedError = gcnew String(errorMsg);
                errors->Release();
                throw gcnew DXException("Failed to compile shader: " + managedError);
            }
            _com_error err(hr);
            LPCTSTR errMsg = err.ErrorMessage();
            String^ managedError = gcnew String(errMsg);
            throw gcnew DXException("Failed to compile shader: " + managedError);
        }

        return gcnew DX12Shader(bytecode, shaderType);
    }
}
