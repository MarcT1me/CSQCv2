using System.Reflection;
using System.Runtime.InteropServices;

namespace QuantumLauncher;

internal class EngineCoreModule : QuantumModule
{
    public EngineCoreModule(bool isHeadless, string rootPath)
    {
        Assembly = Assembly.Load("QuantumEngine.Core");

        GetAssemblyProp("Engine.Configuration.BaseConfig", "AppName")?
            .SetValue(null, QLauncher.AppLibModule.Domain.FriendlyName);
        GetAssemblyProp("Engine.Configuration.BaseConfig", "Headless")?
            .SetValue(null, isHeadless);
        GetAssemblyProp("Engine.QuantumEngine.Core", "RootDirectory")?
            .SetValue(null, rootPath);
    }
}

public class EngineModule : QuantumModule
{
    public static string NativeLibsPath = "runtimes";
    public static string EngineBinariesPath = "Engine";

    private readonly string _baseClassName;

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

        _baseClassName = isHeadless ? "HEngineCore" : "QEngineCore";
        var name = isHeadless ? "HeadlessQuantum" : "Quantum";
        Console.WriteLine(
            $"Loading {name} Engine Module"
        );
        Assembly = Assembly.Load(name + "Engine");
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
        InvokeAssemblyMethod($"Engine.{_baseClassName}", "Initialize", QLauncher.AppLibModule.Assembly);
    }

    public void Deactivate()
    {
        Console.WriteLine(
            "Deactivate EngineModule"
        );
        InvokeAssemblyMethod($"Engine.{_baseClassName}", "Uninitialize");
    }
}