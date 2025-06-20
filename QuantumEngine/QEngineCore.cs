using System.Reflection;
using MirageAPI;

namespace Engine;

using Extensions;
using Failures;
using Graphic.OpenGl;
using Logging;

public class QEngineCore(Assembly? appLibAssembly) : EngineCore(appLibAssembly)
{
    [Obsolete("ENGINE ONLY USAGE")]
    public static void Initialize(Assembly? appLibAssembly)
    {
        With.Handle(new Catch("Main EngineCore Catch"), _ =>
        {
            var core = new QEngineCore(appLibAssembly);

            // initialize core
            core.InitializeCore();
        });
    }

    protected override void InitializeModeSpecific()
    {
        Logger.Info("Initialize MirageAPI");
        if (!MirageSystem.Init(GlData.ApiVersions.X, GlData.ApiVersions.Y))
        {
            Logger.Error("Failed to initialize MirageAPI");
        }
        
        MirageAPI.Events.NativeEventManager.Initialize();
    }

    protected override void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }

    [Obsolete("ENGINE ONLY USAGE")]
    public void Uninitialize()
    {
        Logger.Info("Uninitialize EngineCore");

        Logger.Info("Uninitialize MirageAPI");
        MirageSystem.Shutdown();

        Logger.Success("EngineCore uninitialized");
    }
}