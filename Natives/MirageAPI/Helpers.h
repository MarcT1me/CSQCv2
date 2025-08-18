#pragma once

#define QuantumFailure Engine::Failures::FailureException

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

#define QIdentifier Engine::Data::Identifier
#define QLogger Engine::Logging::Logger
#define QLog(logLevel, msg) QLogger::logLevel((msg))

#define CSStringToWString(string) msclr::interop::marshal_as<std::wstring>((string))
#define CStringToWChar(string) CSStringToWString(string).c_str()
