using System.Reflection;

namespace QuantumLauncher;

public class AppLibModule : QuantumModule
{
    public string MainClassName;
    public AppDomain Domain;

    private static readonly HashSet<string> EngineBinaries =
    [
        "Engine.dll"
    ];

    public AppLibModule(AppDomain domain, string mainClassName)
    {
        MainClassName = mainClassName;
        Domain = domain;

        Domain.AssemblyResolve += CurrentDomain_AssemblyResolve!;
        Console.WriteLine(
            "Assembly Resolver - added"
        );

        Console.WriteLine(
            "Loading AppLib"
        );
        Assembly = Assembly.Load(MainClassName);
    }

    private static Assembly? CurrentDomain_AssemblyResolve(object sender, ResolveEventArgs args)
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

        return File.Exists(fullPath) ? Assembly.LoadFrom(fullPath) : null;
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
        InvokeAssemblyMethod($"{MainClassName}.{MainClassName}", "Start");
    }
}