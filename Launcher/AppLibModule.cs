using System.Reflection;

namespace QuantumLauncher;

public class AppLibModule : QuantumModule
{
    public string MainName;
    public AppDomain Domain;

    private static readonly HashSet<string> EngineBinaries =
    [
        "QuantumEngine.Core.dll", "QuantumEngine.dll", "MirageAPI.dll"
    ];

    public AppLibModule(AppDomain domain, string mainName)
    {
        MainName = mainName;
        Domain = domain;

        Domain.AssemblyResolve += CurrentDomain_AssemblyResolve!;
        Console.WriteLine(
            "Assembly Resolver - added"
        );

        Console.WriteLine(
            "Loading AppLib"
        );
        Assembly = Assembly.Load(MainName);
    }

    private static Assembly CurrentDomain_AssemblyResolve(object sender, ResolveEventArgs args)
    {
        var assemblyName = new AssemblyName(args.Name).Name + ".dll";

        var assemblyPath = Path.Combine(
            QLauncher.BinariesPath,
            IsEngineBinary(assemblyName) ? EngineModule.EngineBinariesPath : "",
            assemblyName
        );
        var fullPath = Path.Combine(
            QLauncher.RootDir,
            assemblyPath
        );

        Console.WriteLine(
            $"Load new Assembly: {assemblyName} - {assemblyPath}"
        );

        return File.Exists(fullPath)
            ? Assembly.LoadFrom(fullPath)
            : throw new Exception($"Could not find assembly {assemblyPath}");
    }

    private static bool IsEngineBinary(string assemblyName)
    {
        return EngineBinaries.Contains(assemblyName);
    }

    public void Start()
    {
        Console.WriteLine(
            "Start AppLib"
        );
        InvokeAssemblyMethod($"{MainName}.{MainName}", "Start");
    }
}