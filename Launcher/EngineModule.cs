using System.Reflection;
using System.Runtime.InteropServices;

namespace QuantumLauncher;

internal class EngineCoreModule : QuantumModule
{
    public EngineCoreModule(bool isHeadless)
    {
        Assembly = Assembly.Load("EngineCore");
        
        GetAssemblyProp("Engine.Configuration.BaseConfig", "AppName")?
            .SetValue(null, QLauncher.AppLibModule.Domain.FriendlyName);
        GetAssemblyProp("Engine.Configuration.BaseConfig", "Headless")?
            .SetValue(null, isHeadless);
    }
}

public class EngineModule : QuantumModule
{
    public static string NativeLibsPath = "runtimes";
    public static string EngineBinariesPath = "Engine";

    private readonly string _baseClassName;

    public EngineModule(bool isHeadless)
    {
        Console.WriteLine(
            "Loading EngineModule"
        );

        _ = new EngineCoreModule(isHeadless);
        
        Console.WriteLine(
            "Loading Native runtimes"
        );
        // LoadNative("SDL2");
        LoadNative("freetype6");

        var assemblyName = isHeadless ? "HeadlessQuantumEngine" : "QuantumEngine";
        _baseClassName = isHeadless ? "HEngineCore" : "QEngineCore";
        
        Assembly = Assembly.Load(assemblyName);

        GetAssemblyProp($"Engine.{_baseClassName}", "RootDirectory")?
            .SetValue(null, QLauncher.RootDir);
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