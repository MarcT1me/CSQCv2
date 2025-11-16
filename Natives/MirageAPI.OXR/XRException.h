#pragma once

#include <string>

namespace MirageAPI::Native::OpenXR
{
    class XRException : public std::exception
    {
    protected:
        std::string _message;
        std::string _fullMessage;

    public:
        XRException(std::string message)
            : _message(std::move(message)),
              _fullMessage("OpenXR error: " + _message)
        {
        }

        std::string GetMessage() const { return _message; }

        const char* what() const noexcept override
        {
            return _fullMessage.c_str();
        }
    };

    inline std::string to_string(const XRException& e)
    {
        return e.what();
    }

    class XRResultException : public XRException
    {
    protected:
        XrResult _result;

    public:
        XRResultException(const std::string& context, XrResult result)
            : XRException("Result: " + std::to_string(result) + " in " + context),
              _result(result)
        {
        }

        XrResult GetXRResult() const { return _result; }
    };

    class XRResultChecker
    {
    public:
        static void Check(XrResult result, const char* context)
        {
            if (XR_FAILED(result))
            {
                throw XRResultException(context, result);
            }
        }

        static void Check(XrResult result, const std::string& context)
        {
            Check(result, context.c_str());
        }
    };
}
