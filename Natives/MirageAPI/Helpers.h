#pragma once

#define QuantumFailure Engine::Failures::FailureException

#define CheckMissmatch(first, second) if ((first) != (second))
#define CheckNull(resource) if (!(resource))
#define ThrowIfNull(resource, msg) CheckNull(resource) throw gcnew MirageAPI::NullptrException((msg))

#define SimpleDelete(resource) if ((resource)) { delete (resource); (resource) = nullptr; }
#define SimpleDeleteArr(resource) if ((resource)) { delete[] (resource); (resource) = nullptr; }
#define SimpleRelease(resource) if ((resource)) { (resource)->Release(); (resource) = nullptr; }

#define CSFormat System::String::Format
#define CSList System::Collections::Generic::List

#define QuantumLogger Engine::Logging::Logger::
#define QuantumLog(logLevel, msg) QuantumLogger logLevel((msg))
