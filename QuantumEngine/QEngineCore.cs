using System.Reflection;

namespace Engine;

using Extensions.Tracer;
using Extensions;
using Failures;
using Logging;

public class QEngineCore(Assembly? appLibAssembly) : EngineCore(appLibAssembly)
{
    [Obsolete("ENGINE ONLY USAGE")]
    public static void Initialize(Assembly? appLibAssembly)
    {
        With.Handle(new Catch("Main EngineCore Catch"), _ =>
        {
            QuantumTracer.HandleAssembly(
                [
                    Assembly.GetExecutingAssembly()
                ]
            );

            var core = new QEngineCore(appLibAssembly);

            // initialize core
            core.InitializeCore();
        });
    }

    protected override void InitializeModeSpecific()
    {
        Logger.Info("Initialize MirageAPI::DirectX12");
        MirageAPI.DirectX.DX12Context.Initialize();
    }

    protected override void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }

    [Obsolete("ENGINE ONLY USAGE")]
    public static void Uninitialize()
    {
        Logger.Info("Deinitialize MirageAPI::DirectX12");
        MirageAPI.DirectX.DX12Context.Deinitialize();
        
        Logger.Separator();
        Logger.Success("QuantumEngine uninitialized");
    }
}