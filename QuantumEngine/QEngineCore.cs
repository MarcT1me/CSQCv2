using System.Reflection;
using Engine.Asset;
using Engine.Asset.Defaults;
using Engine.UI;
using Microsoft.Toolkit.Uwp.Notifications;
using MirageAPI.DirectX;

namespace Engine;

using Extensions.Tracer;
using Extensions;
using Failures;
using Logging;

internal sealed class ErrorMessage(Catch cth, Exception failure) : ToastNotification
{
    public override void Build(ToastContentBuilder builder)
    {
        builder
            .AddText("Engine got critical exception")
            .AddText($"cth: \'{cth.Id}\'")
            .AddText($"failure: \'{failure.Message}\'")
            .AddAttributionText($"type: \'{failure.GetType()}\'")
            .AddButton("🔄️ Restart", ToastActivationType.Foreground, "", null);
    }
}

public class QEngineCore : EngineCore
{
    public static Engine.Asset.Default.Configuration QConfiguration = new(
        "QuantumEngine:engine.configuration.properties"
    );

    [Obsolete("ENGINE ONLY USAGE")]
    public static void Initialize(Assembly? appLibAssembly)
    {
        With.Handle(
            new Catch("QuantumEngine Initialization Catch")
                .Except<Exception>((cth, failure) =>
                    {
                        Logger.Warning("try to send notification");
                        new ErrorMessage(cth, failure).Show();
                        DefaultFailureHandler.OnFailure(new FailureException(failure.Message, failure));
                    }
                ),
            _ =>
            {
                Core.Initialize(appLibAssembly);

                Logger.Separator();

                DebugFeatures();

                Logger.Separator();

                ModeSpecific();

                Logger.Separator();
                Logger.Success("Quantum Engine module initialized");
                Logger.Separator();
            }
        );
    }

    private static void DebugFeatures()
    {
        foreach (
            var assembly in (List<Assembly?>)
            [
                (Assembly?)QuantumTracer.GetScanned("TestApp", ScanTypes.Assembly),
                (Assembly?)QuantumTracer.GetScanned("AppLib", ScanTypes.Assembly),
                (Assembly?)QuantumTracer.GetScanned("QuantumEngine.Core", ScanTypes.Assembly),
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

    private static void ModeSpecific()
    {
        Logger.Info("Initialize MirageAPI::DirectX12");

        string flagNames = QConfiguration.Get<string>("engine.directx.device.adapter");

        DX12DeviceInitFlags flags = (DX12DeviceInitFlags)Enum.Parse(
            typeof(DX12DeviceInitFlags), "Use" + flagNames, false
        );
        flags |= IsDebug ? DX12DeviceInitFlags.Debug : DX12DeviceInitFlags.None;

        DX12Device.Initialize(flags);

        AssetManager.RegisterAssetType(new AssetType("image", new ImageAssetLoader()));
        AssetManager.RegisterAssetType(new AssetType("vertexShader", new VertexShaderAssetLoader()));
        AssetManager.RegisterAssetType(new AssetType("pixelShader", new PixelShaderAssetLoader()));
    }

    public static T EngConfig<T>(string name)
    {
        if (QConfiguration.Contains(name))
            return QConfiguration.Get<T>(name);
        if (Core.Configuration.Contains(name))
            return Core.Configuration.Get<T>(name);
        throw new Asset.Default.Configuration.ConfigurationError(name);
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