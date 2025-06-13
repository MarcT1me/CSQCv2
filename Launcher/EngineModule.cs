using System.Reflection;
using System.Runtime.InteropServices;

namespace QuantumLauncher;

public class EngineModule : QuantumModule
{
    public static string NativeLibsPath = "runtimes";
    public static string EngineBinariesPath = "Engine";

    public EngineModule(bool isHeadless)
    {
        Console.WriteLine(
            "Loading EngineModule"
        );
        
        Console.WriteLine(
            "Loading Native runtimes"
        );
        LoadNative("SDL2");
        LoadNative("freetype6");
        
        Assembly = Assembly.Load("Engine");

        GetAssemblyProp("Engine.EngineCore", "RootDirectory")?
            .SetValue(null, QLauncher.RootDir);
        GetAssemblyProp("Engine.Configuration.BaseConfig", "AppName")?
            .SetValue(null, QLauncher.AppLibModule.Domain.FriendlyName);
        GetAssemblyProp("Engine.Configuration.BaseConfig", "Headless")?
            .SetValue(null, isHeadless);
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
        InvokeAssemblyMethod("Engine.EngineCore", "Initialize", QLauncher.AppLibModule.Assembly);
    }
}