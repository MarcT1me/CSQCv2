using System.Reflection;
using System.Runtime.InteropServices;

namespace QuantumLauncher;

internal class EngineCoreModule : QuantumModule
{
    public EngineCoreModule(bool isHeadless, string rootPath)
    {
        Assembly = Assembly.Load("QuantumEngine.Core");
    }
}

public class EngineModule : QuantumModule
{
    public static string NativeLibsPath = "runtimes";
    public static string EngineBinariesPath = "Engine";

    public EngineModule(bool isHeadless, string rootPath)
    {
        Console.WriteLine(
            "Loading Core Engine Module"
        );

        _ = new EngineCoreModule(isHeadless, rootPath);

        Console.WriteLine(
            "Loading Native runtimes"
        );
        LoadNative("freetype6");
        LoadNative("Ijwhost");

        Assembly = Assembly.Load("QuantumEngine");
    }

    private void LoadNative(string name)
    {
        string path = Path.Combine(QLauncher.BinariesPath, NativeLibsPath, name + ".dll");
        Console.WriteLine("Load Native: " + path);
        string fullPath = Path.Combine(QLauncher.RootDir, path);
        NativeLibrary.Load(fullPath);
    }

    public void Activate()
    {
        Console.WriteLine(
            "Activate EngineModule"
        );
        InvokeAssemblyMethod("QuantumCore.Engine", "Initialize", QLauncher.AppLibModule.Assembly);
    }

    public void Deactivate()
    {
        Console.WriteLine(
            "Deactivate EngineModule"
        );
        InvokeAssemblyMethod("QuantumCore.Engine", "Uninitialize");
    }
}