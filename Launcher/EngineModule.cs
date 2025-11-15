using System.Reflection;
using System.Runtime.InteropServices;

namespace QuantumLauncher;

internal class EngineCoreModule : QuantumModule
{
    public EngineCoreModule()
    {
        Console.Write("Loading Engine Core Module ");
        Assembly = Assembly.Load("QuantumEngine.Core");

        Console.WriteLine("Setting-up Engine Core Module");
        GetAssemblyProp("QuantumCore.Core", "RootDirectory")?
            .SetValue(null, QLauncher.RootDir);
    }
}

public class EngineModule : QuantumModule
{
    private void LoadNative(string name)
    {
        string path = Path.Combine(QLauncher.BinariesPath, NativeLibsPath, name + ".dll");
        string fullPath = Path.Combine(QLauncher.RootDir, path);

        Console.WriteLine("Load Native: " + path);
        NativeLibrary.Load(fullPath);
    }

    public EngineModule()
    {
        _ = new EngineCoreModule();

        Console.WriteLine("Loading Native runtimes for Client Module");
        LoadNative("freetype6");
        LoadNative("Ijwhost");
        LoadNative("openxr_loader");

        Console.Write("Loading Engine Client Module ");
        Assembly = Assembly.Load("QuantumEngine");
    }

    public void Activate()
    {
        Console.WriteLine("Activate EngineModule");
        InvokeAssemblyMethod("QuantumCore.Engine", "Initialize", QLauncher.AppLibModule.Assembly);
    }

    public void Deactivate()
    {
        Console.WriteLine("Deactivate EngineModule");
        InvokeAssemblyMethod("QuantumCore.Engine", "Uninitialize");
    }
}