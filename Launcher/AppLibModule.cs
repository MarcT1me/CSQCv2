using System.Reflection;

namespace QuantumLauncher;

public class AppLibModule : QuantumModule
{
    public string MainName;
    public AppDomain Domain;

    public static string EngineBinariesPath = "Engine";

    private static readonly HashSet<string> EngineBinaries =
    [
        "QuantumEngine.Core.dll", "QuantumEngine.dll", "MirageAPI.dll"
    ];

    public AppLibModule(AppDomain domain, string mainName)
    {
        MainName = mainName;
        Domain = domain;

        Console.WriteLine("Assembly Resolver");
        Domain.AssemblyResolve += CurrentDomain_AssemblyResolve!;

        Console.Write("Loading AppLib ");
        Assembly = Assembly.Load(MainName);
    }

    private static bool IsEngineBinary(string assemblyName)
    {
        return EngineBinaries.Contains(assemblyName);
    }

    private static Assembly CurrentDomain_AssemblyResolve(object sender, ResolveEventArgs args)
    {
        var assemblyName = new AssemblyName(args.Name).Name + ".dll";

        var assemblyPath = Path.Combine(
            QLauncher.BinariesPath,
            IsEngineBinary(assemblyName) ? EngineBinariesPath : "",
            assemblyName
        );
        var fullPath = Path.Combine(
            QLauncher.RootDir,
            assemblyPath
        );

        Console.WriteLine($"Assembly: {assemblyName} - {assemblyPath}");
        return File.Exists(fullPath)
            ? Assembly.LoadFrom(fullPath)
            : throw new Exception($"Could not find assembly {assemblyPath}");
    }

    public void Start()
    {
        Console.WriteLine(
            "Start AppLib"
        );
        InvokeAssemblyMethod($"{MainName}.{MainName}", "Start");
    }
}