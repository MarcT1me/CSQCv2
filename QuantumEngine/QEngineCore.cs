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

        DX12DeviceInitFlags flags = DX12DeviceInitFlags.UseHighPerformanceAdapter;
        flags |= BaseConfig.DebugMode ? DX12DeviceInitFlags.Debug : DX12DeviceInitFlags.None;

        DX12Device.Initialize(flags);

        AssetManager.RegisterAssetType(new AssetType("image", new ImageAssetLoader()));
        AssetManager.RegisterAssetType(new AssetType("vertexShader", new VertexShaderAssetLoader()));
        AssetManager.RegisterAssetType(new AssetType("pixelShader", new PixelShaderAssetLoader()));
    }

    protected override void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");

        foreach (
            var assembly in (List<Assembly?>)
            [
                (Assembly?)QuantumTracer.GetScanned("AppLib", ScanTypes.Assembly),
                (Assembly?)QuantumTracer.GetScanned("EngineCore", ScanTypes.Assembly),
                (Assembly?)QuantumTracer.GetScanned("QuantumEngine", ScanTypes.Assembly)
            ]
        )
        {
            if (assembly == null) continue;

            var assemblyName = assembly.GetName().Name;
            Logger.Info($"Embed Resources for {assemblyName}:");
            foreach (var name in assembly.GetManifestResourceNames())
            {
                Logger.SimpleLog($"{assemblyName}:{name}");
            }
        }
    }

    [Obsolete("ENGINE ONLY USAGE")]
    public static void Uninitialize()
    {
        Logger.Info("Deinitialize MirageAPI::DirectX12");
        DX12Device.Deinitialize();
        Logger.Separator();
        Logger.Success("QuantumEngine uninitialized");
    }
}