using Engine.Decorators;
using Engine.Extensions.DebugFeatures;
using Engine.Extensions.Tracer;
using Engine.Logging;

namespace AppLib;

public class Game
{
    public static void StartGame()
    {
        var app = new TestClass();

        var ret = app.TestLogMethod();
        Logger.Debug(ret);

        if (QuantumTracer.GetScanned("TestCallback", ScanTypes.Callback) is QuantumMethodInfo methodInfo)
        {
            Logger.Debug(methodInfo.Method.Invoke(app, [1, 2])?.ToString() ?? String.Empty);
        }

        if (QuantumTracer.GetScanned("TestCallback2", ScanTypes.Callback) is QuantumMethodInfo methodInfo2)
        {
            Logger.Debug(methodInfo2.Method.Invoke(app, [1, 2])?.ToString() ?? String.Empty);
        }
    }
}

[QTrace(ScanTypes.Scan)]
public class TestClass
{
    [Log("test logging")]
    public string TestLogMethod()
    {
        return "some string";
    }

    [QTrace(ScanTypes.Callback)]
    public int TestCallback(int x, int y)
    {
        return x * y;
    }

    public int TestCallback2(int x, int y)
    {
        return x + y;
    }
}