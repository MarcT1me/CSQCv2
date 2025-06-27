#include "pch.h"
#include "DX12Shader.h"

namespace MirageAPI::DirectX
{
    DX12Shader::DX12Shader(
        array<System::Byte>^ bytecode,
        DX12ShaderType type
    ) : m_bytecode(bytecode),
        m_type(type)
    {
    }

    D3D12_SHADER_BYTECODE DX12Shader::GetNativeBytecode()
    {
        pin_ptr<System::Byte> pinnedData = &m_bytecode[0];
        D3D12_SHADER_BYTECODE bytecode;
        bytecode.pShaderBytecode = pinnedData;
        bytecode.BytecodeLength = m_bytecode->Length;
        return bytecode;
    }
}
