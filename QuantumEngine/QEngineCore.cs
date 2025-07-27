using System.Reflection;
using Engine.Asset;
using Engine.Asset.Defaults;
using Engine.Configuration;
using MirageAPI.DirectX;

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
            QuantumTracer.HandleAssembly([Assembly.GetExecutingAssembly()]);
            var core = new QEngineCore(appLibAssembly);
            core.InitializeCore();
        });
    }

    protected override void InitializeModeSpecific()
    {
        Logger.Info("Initialize MirageAPI::DirectX12");

        DX12ContextInitFlags flags = DX12ContextInitFlags.None;
        flags |= BaseConfig.DebugMode ? DX12ContextInitFlags.Debug : DX12ContextInitFlags.None;
        flags |= DX12ContextInitFlags.UseAdapter | DX12ContextInitFlags.UseHighPerformanceAdapter;

        DX12Context.Initialize(flags);

        AssetManager.RegisterAssetType(new AssetType("image", new ImageAssetLoader()));
    }

    protected override void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }

    [Obsolete("ENGINE ONLY USAGE")]
    public static void Uninitialize()
    {
        Logger.Info("Deinitialize MirageAPI::DirectX12");
        DX12Context.Deinitialize();
        Logger.Separator();
        Logger.Success("QuantumEngine uninitialized");
    }
}