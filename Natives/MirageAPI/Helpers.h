#pragma once

#define QuantumFailure QuantumCore::Failures::FailureException

#define CheckMissmatch(first, second) if ((first) != (second))
#define CheckNull(resource) if (!(resource))
#define ThrowIfNull(resource, msg) CheckNull(resource) throw gcnew MirageAPI::NullptrException((msg))

#define SimpleDelete(resource) if ((resource)) { delete (resource); (resource) = nullptr; }
#define SimpleDeleteArr(resource) if ((resource)) { delete[] (resource); (resource) = nullptr; }
#define SimpleRelease(resource) if ((resource)) { (resource)->Release(); (resource) = nullptr; }
#define SafeDestroy(destroyFunc, resource) if ((resource)) { destroyFunc((resource)); resource = nullptr; }

#define CSFormat System::String::Format
#define CSList System::Collections::Generic::List
#define CSDict System::Collections::Generic::Dictionary

#define QIdentifier QuantumCore::Data::Identifier
#define QLogger QuantumCore::Logging::Logger
#define QLog(logLevel, msg) QLogger::logLevel((msg))

#define CStringToWString(str) msclr::interop::marshal_as<std::wstring>((str))
#define CStringToWChar(str) CStringToWString(str).c_str()

#define CStringToString(str) msclr::interop::marshal_as<std::string>((str))
#define CStringToChar(str) CStringToString(str).c_str()

#define StringToAnsiPtr(str) Marshal::StringToHGlobalAnsi((str))
#define FreeAnsiPtr(ptr) Marshal::FreeHGlobal((IntPtr)(ptr))

#define UnpacVec2(vector) vector->X, vector->Y
#define UnpacVec4(vector) UnpacVec2(vector), vector->Z, vector->W


namespace MirageAPI
{
    public ref class GlobalHelpers abstract
    {
    public:
        static bool IsWindowsVersion(UINT major, UINT minor, UINT build)
        {
            // create version
            OSVERSIONINFOEXW osVersionInfo = {sizeof(osVersionInfo)};
            osVersionInfo.dwMajorVersion = major;
            osVersionInfo.dwMinorVersion = minor;
            osVersionInfo.dwBuildNumber = build;

            // verify with using mask
            DWORDLONG const mask = VerSetConditionMask(
                VerSetConditionMask(
                    VerSetConditionMask(
                        0,
                        VER_MAJORVERSION, VER_GREATER_EQUAL
                    ),
                    VER_MINORVERSION, VER_GREATER_EQUAL
                ),
                VER_BUILDNUMBER, VER_GREATER_EQUAL
            );

            return VerifyVersionInfoW(
                &osVersionInfo,
                VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER,
                mask
            );
        }
    };
}
