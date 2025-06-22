using System.Reflection;
using MirageAPI;
using MirageAPI.Events;
using MirageAPI.Vulkan;

namespace Engine;

using Events;
using Extensions.Tracer;
using Extensions;
using Failures;
using Logging;

public class QEngineCore(Assembly? appLibAssembly) : EngineCore(appLibAssembly)
{
    public static VulkanPipeline? VulkanPipeline;

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
        Logger.Info("Initialize MirageAPI");
        MirageSystem.Initialize(initVulkan: false, initOpenGl: true);

        Logger.Info("Initialize MirageAPI::Events");
        NativeEventManager.Initialize();

        Logger.Info("Initialize Engine.Events");
        QEventSystem.Initialize();

        if (MirageSystem.initVulkan)
        {
            Logger.Info("Initialize MirageAPI::Vulkan");
            MirageSystem.InitVulkan();

            Logger.Separator();

            Logger.Info("Initialize MirageAPI::Vulkan::VulkanPipeline");
            VulkanPipeline = new VulkanPipeline(MirageSystem.CurrentContext);
            if (MirageSystem.CurrentContext.IsRTXSupported)
            {
                Logger.Info(
                    "PIPELINE:\n" +
                    "Mode: RTX"
                );
                VulkanPipeline.CreateRTXPipeline();
            }
            else
            {
                Logger.Info(
                    "PIPELINE:\n" +
                    "Mode: Simple"
                );
                VulkanPipeline.CreateGraphicsPipeline();
            }
        }

        NativeEventManager.Initialize();
        Logger.Success("MirageAPI - Initialized");

        Logger.Separator();
    }

    protected override void EnableDebugFeatures()
    {
        Logger.Debug("Enable Debug Features");
    }

    [Obsolete("ENGINE ONLY USAGE")]
    public static void Uninitialize()
    {
        if (MirageSystem.initVulkan)
        {
            Logger.Info("Uninitialize MirageAPI::Vulkan");
            MirageSystem.DeinitializeVulkan();
        }

        Logger.Info("Uninitialize MirageAPI");
        MirageSystem.Deinitialize();

        Logger.Separator();
        Logger.Success("QuantumEngine uninitialized");
    }
}