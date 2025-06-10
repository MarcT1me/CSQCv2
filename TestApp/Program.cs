using System.Reflection;
using Engine;
using Engine.Extensions.DebugFeatures;
using Engine.Logging;

namespace TestApp;

public partial class Program
{
    [Obsolete("Obsolete")]
    public static void Main(string[] args)
    {
        EngineCore.Initialize(
            appLibAssembly: Assembly.GetExecutingAssembly()
        );
        
        var app = new Program();
        var ret = app.TestLogMethod();
        Logger.Debug(ret.ToString() ?? string.Empty);
    }

    [Log("ХУЙ")]
    public int? TestLogMethod()
    {
        return 120;
    }
}