#pragma warning disable CS8618 // Non-nullable field must contain a non-null value when exiting constructor. Consider adding the 'required' modifier or declaring as nullable.

namespace QuantumLauncher;

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
            EngineModule = new EngineModule(isHeadless)
        );
    }
}