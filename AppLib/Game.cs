using Engine.Data;
using Engine.Decorators;
using Engine.Extensions.Tracer;
using Engine.Logging;
using Engine.Objects.Light;
using OpenTK.Mathematics;

namespace AppLib;

public class Game
{
    public static void StartGame()
    {
        var app = new TestClass();
        Logger.Separator();
        Logger.Separator();

        if (QuantumTracer.GetScanned("TestCallback", ScanTypes.Callback) is QuantumMethodInfo methodInfo)
        {
            Logger.Debug("Found TestCallback method");
            Logger.Debug(methodInfo.Method.Invoke(app, [1, 2])?.ToString() ?? String.Empty);
        }

        Logger.Info("Calling TestLogMethod...");
        var ret = TestClass.TestLogMethod();
        Logger.Debug($"TestLogMethod returned: {ret}");

        Logger.Info("Calling instance method...");
        var result = app.TestInstanceMethod("Hello", "World");
        Logger.Debug($"Instance method returned: {result}");

        var light = new PointLightData(new Color4(), 1.0f, identifier: "TestPointLightData")
        {
            Transform = new Transform(),
            CastShadows = false,
            MaxDistance = 50f
        };
        Logger.Separator();
        Logger.Debug(
            $"Light: '{light.Identifier}', '{light.LightType}', '{light.Status}', {light.Color}, {light.MaxDistance}"
        );
    }
}

[QTrace(ScanTypes.Scan)]
public class TestClass
{
    public static string TestLogMethod()
    {
        return "some string";
    }

    public string TestInstanceMethod(string a, string b)
    {
        Logger.Debug($"Inside TestInstanceMethod - concatenating '{a}' and '{b}'");
        return a + " " + b;
    }

    [QTrace(ScanTypes.Callback)]
    public static int TestCallback(int x, int y)
    {
        Logger.Debug($"Inside TestCallback - calculating {x} * {y}");
        return x * y;
    }
}