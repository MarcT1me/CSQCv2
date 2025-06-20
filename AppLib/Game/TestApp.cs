using Engine.Base;
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

    // public override void HandleEvent(QuantumEvent e)
    // {
    //     base.HandleEvent(e);
    //     Logger.Debug($"event type {e.Type}");
    // }
}