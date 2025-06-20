using Engine.Base;
using Engine.Events.QuantumEvents;
using Engine.Logging;

// using Engine.Events.QuantumEvents;

namespace AppLib.Game;

public class TestApp : Game<TestAppData, GameWindow>
{
    public override TestAppData PrepareInstance()
    {
        return new TestAppData();
    }

    protected override GameWindow CreateMainWindow()
    {
        var n = MetaData.Identifier.GetNameAnyway();
        return new GameWindow(MetaData.WinData, MetaData.GlData, n);
    }

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);
        if (e != null)
            Logger.Info($"Got event: {e.Type}");
    }
}