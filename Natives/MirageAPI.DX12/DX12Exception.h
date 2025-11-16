#pragma once

#include <string>

namespace MirageAPI::Native::DirectX12
{
    class DX12Exception : public std::exception
    {
    protected:
        std::string _message;
        std::string _fullMessage;

    public:
        DX12Exception(std::string message)
            : _message(std::move(message)),
              _fullMessage("DirectX12 error: " + _message)
        {
        }

        std::string GetMessage() const { return _message; }

        const char* what() const noexcept override
        {
            return _fullMessage.c_str();
        }
    };

    inline std::string to_string(const DX12Exception& e)
    {
        return e.what();
    }

    class DX12ResultException : public DX12Exception // NOLINT(clang-diagnostic-padded)
    {
    protected:
        HRESULT _result;

    public:
        DX12ResultException(const std::string& context, HRESULT result)
            : DX12Exception("Result: " + std::to_string(result) + " in " + context),
              _result(result)
        {
        }

        HRESULT GetResult() const { return _result; }
    };

    class DX12ResultChecker
    {
    public:
        static void Check(HRESULT result, const char* context)
        {
            if (FAILED(result))
            {
                if (result == DXGI_ERROR_DEVICE_REMOVED)
                {
                    throw DX12ResultException(
                        std::string(context) + " (Device Removed: " + std::to_string(DX12Device::GetDeviceRemovedReason()) + ")",
                        result
                    );
                }
                if (result == E_INVALIDARG)
                {
                    throw DX12ResultException(
                        std::string(context) + " (Invalid Arguments)",
                        result
                    );
                }
                if (result == E_OUTOFMEMORY)
                {
                    throw DX12ResultException(
                        std::string(context) + " (Out Of Memory)",
                        result
                    );
                }
                throw DX12ResultException(
                    context,
                    result
                );
            }
        }
    };
}
