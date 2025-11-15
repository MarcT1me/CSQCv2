namespace AppLib;

using Game;

public static class AppLib
{
    // static AppLib()
    // {
    //     XRInstanceConfig cfg = new("Test QC App", 1);
    //     cfg.EnabledExtensions.Add("XR_KHR_D3D12_enable");
    //     if (QuantumCore.Core.IsDebug) cfg.EnabledExtensions.Add("XR_EXT_debug_utils");
    //
    //     QuantumCore.Engine.VrInstance = new(cfg);
    // }

    public static void Start()
    {
        TestApp.Mainloop(typeof(TestApp));
    }
}