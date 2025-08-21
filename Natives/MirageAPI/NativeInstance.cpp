#include "pch.h"
#include "NativeInstance.h"

namespace MirageAPI
{
    // constructor for MirageAPI instance
    NativeInstance::NativeInstance(
        String^ name,
        HINSTANCE hInstance
    ) : instanceName(name),
        hInstance(hInstance)
    {
        instances[instanceName] = this; // register
    }

    // for C# assemblies
    NativeInstance::NativeInstance(
        Reflection::Assembly^ assembly
    ) : instanceName(assembly->GetName()->Name),
        hInstance(static_cast<HINSTANCE>(
            Runtime::InteropServices::Marshal::GetHINSTANCE(assembly->GetModules()[0]).ToPointer()
        ))
    {
        instances[instanceName] = this; // register
    }

    NativeInstance^ NativeInstance::GetForAssembly(Reflection::Assembly^ assembly)
    {
        // check empty instance and MirageAPI
        if (assembly == nullptr)
            return System;

        // try to get instance
        NativeInstance^ instance;
        instances.TryGetValue(assembly->GetName()->Name, instance);

        // if exist - just return, else - create and register
        if (instance)
            return instance;

        return gcnew NativeInstance(assembly);
    }
}
