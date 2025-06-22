using Engine.Base;
using Engine.Events.QuantumEvents;
using Engine.Events.QuantumEvents.Joystick;
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
}