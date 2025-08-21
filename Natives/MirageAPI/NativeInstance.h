#pragma once

namespace MirageAPI
{
    public ref class NativeInstance
    {
        static CSDict<String^, NativeInstance^> instances;  // list of all registered assemblies

        // constructor for MirageAPI instance
        NativeInstance(
            String^ name,
            HINSTANCE hInstance
        );

        // for C# assemblies
        NativeInstance(
            Reflection::Assembly^ assembly
        );

    internal:
        // instance data
        String^ instanceName;
        HINSTANCE hInstance;

    public:
        // Empty instance - used for system resources
        static initonly NativeInstance^ System = gcnew NativeInstance(
            "System",
            nullptr
            );
        // default instance - Current libreary
        static initonly NativeInstance^ MirageAPI = gcnew NativeInstance(
            "MirageAPI",
            GetModuleHandleW(nullptr)
        );

        // get instance for assembly. Register new if its needs
        static NativeInstance^ GetForAssembly(Reflection::Assembly^ assembly);
    };
}
