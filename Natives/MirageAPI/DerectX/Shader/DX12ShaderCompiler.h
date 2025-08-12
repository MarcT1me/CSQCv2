#pragma once

#include "DX12Shader.h"

namespace MirageAPI::DirectX::Shader
{
    public ref class DX12ShaderCompiler
    {
    public:
        static DX12Shader^ CompileShaderFromFile(
            const wchar_t* filename,
            DX12ShaderType shaderType,
            const char* entry,
            const char* target,
            DX12ShaderCompileFlags compileFlags0,
            DX12ShaderCompileFlags compileFlags1,
            const D3D_SHADER_MACRO* defines
        );

        static DX12Shader^ CompileShaderFromFile(
            String^ filename,
            DX12ShaderType shaderType,
            String^ entry,
            String^ target,
            DX12ShaderCompileFlags compileFlags0
        )
        {
            pin_ptr<const wchar_t> wFilename = PtrToStringChars(filename);
            return CompileShaderFromFile(
                wFilename,
                shaderType,
                msclr::interop::marshal_as<std::string>(entry).c_str(),
                msclr::interop::marshal_as<std::string>(target).c_str(),
                compileFlags0,
                DX12ShaderCompileFlags::None,
                nullptr
            );
        }

        static DX12Shader^ CompileShaderFromSource(
            const std::string& source,
            DX12ShaderType shaderType,
            const char* entry,
            const char* target,
            DX12ShaderCompileFlags compileFlags0,
            DX12ShaderCompileFlags compileFlags1,
            const D3D_SHADER_MACRO* defines
        );

        static DX12Shader^ CompileShaderFromSource(
            String^ source,
            DX12ShaderType shaderType,
            String^ entry,
            String^ target,
            DX12ShaderCompileFlags compileFlags0
        )
        {
            return CompileShaderFromSource(
                msclr::interop::marshal_as<std::string>(source),
                shaderType,
                msclr::interop::marshal_as<std::string>(entry).c_str(),
                msclr::interop::marshal_as<std::string>(target).c_str(),
                compileFlags0,
                DX12ShaderCompileFlags::None,
                nullptr
            );
        }
    };
}
