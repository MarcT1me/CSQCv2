#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider adding the 'required' modifier or declaring as nullable.

using System.Runtime.InteropServices;

namespace QuantumLauncher;

internal static class NativeLibraryLoader
{
    [DllImport("kernel32.dll", SetLastError = true)]
    private static extern bool SetDllDirectory(string lpPathName);

    public static void ConfigureDllLoading()
    {
        try
        {
            var baseDir = AppDomain.CurrentDomain.BaseDirectory;
            var runtimesPath = Path.Combine(baseDir, "bin", "runtimes");
            var enginePath = Path.Combine(baseDir, "bin", "Engine");
            
            var path = Environment.GetEnvironmentVariable("PATH") ?? "";
            var newPath = $"{runtimesPath};{enginePath};{path}";
            Environment.SetEnvironmentVariable("PATH", newPath);
            
            SetDllDirectory(runtimesPath);
            SetDllDirectory(enginePath);
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine($"Error configuring DLL loading: {ex.Message}");
        }
    }
}

public class QLauncher
{
    public static string BinariesPath = "bin";
    public static string RootDir { get; private set; } = "/";
    public static string BinaryDir { get; private set; }
    public static AppLibModule AppLibModule { get; private set; }
    public static EngineModule EngineModule { get; private set; }

    public static (AppLibModule, EngineModule) InitProject(
        AppDomain domain, string mainClassName = "AppLib", bool isHeadless = false)
    {
        NativeLibraryLoader.ConfigureDllLoading();
        
        RootDir = domain.BaseDirectory;
        BinaryDir = Path.Combine(RootDir, BinariesPath);

        Console.WriteLine(
            $"Initializing {mainClassName} Project:\n" +
            $" Domain: {domain.FriendlyName} -> is headless {isHeadless}\n" +
            $" RootDir: {RootDir}\n" +
            $" BinaryDir: {BinaryDir}"
        );

        return (
            AppLibModule = new AppLibModule(domain, mainClassName),
            EngineModule = new EngineModule()
        );
    }
}