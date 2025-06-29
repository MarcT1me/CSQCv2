#pragma once

#include <msclr/marshal_cppstd.h>

#include "DX12Shader.h"

namespace MirageAPI::DirectX
{
    public ref class DX12ShaderCompiler
    {
    public:
        static DX12Shader^ CompileShaderFromFile(
            const wchar_t* filename,
            DX12ShaderType shaderType,
            const char* entryPoint,
            const char* target,
            DX12ShaderCompileFlags compileFlags0,
            DX12ShaderCompileFlags compileFlags1,
            const D3D_SHADER_MACRO* defines
        );

        static DX12Shader^ CompileShaderFromFile(
            System::String^ filename,
            DX12ShaderType shaderType,
            System::String^ entryPoint,
            System::String^ target,
            DX12ShaderCompileFlags compileFlags0,
            DX12ShaderCompileFlags compileFlags1,
            const D3D_SHADER_MACRO* defines
        )
        {
            pin_ptr<const wchar_t> wFilename = PtrToStringChars(filename);
            return CompileShaderFromFile(
                wFilename,
                shaderType,
                msclr::interop::marshal_as<std::string>(entryPoint).c_str(),
                msclr::interop::marshal_as<std::string>(target).c_str(),
                compileFlags0,
                compileFlags1,
                defines
            );
        }

        static DX12Shader^ CompileShaderFromSource(
            const std::string& source,
            DX12ShaderType shaderType,
            const char* entryPoint,
            const char* target,
            DX12ShaderCompileFlags compileFlags0,
            DX12ShaderCompileFlags compileFlags1,
            const D3D_SHADER_MACRO* defines
        );

        static DX12Shader^ CompileShaderFromSource(
            System::String^ source,
            DX12ShaderType shaderType,
            System::String^ entryPoint,
            System::String^ target,
            DX12ShaderCompileFlags compileFlags0,
            DX12ShaderCompileFlags compileFlags1,
            const D3D_SHADER_MACRO* defines
        )
        {
            return CompileShaderFromSource(
                msclr::interop::marshal_as<std::string>(source),
                shaderType,
                msclr::interop::marshal_as<std::string>(entryPoint).c_str(),
                msclr::interop::marshal_as<std::string>(target).c_str(),
                compileFlags0,
                compileFlags1,
                defines
            );
        }
    };
}
