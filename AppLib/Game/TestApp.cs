using Engine.Base;
using Engine.Events.QuantumEvents;
using Engine.Logging;

namespace AppLib.Game;

public class TestApp : Game<TestAppData, GameWindow>
{
    public override TestAppData PrepareInstance()
    {
        return new TestAppData();
    }

    protected override GameWindow CreateMainWindow()
    {
        return new GameWindow(MetaData.WinData, MetaData.GlData, name: MetaData.Identifier.GetNameAnyway());
    }

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);
        Logger.Debug($"event type {e.Type}");
    }
}