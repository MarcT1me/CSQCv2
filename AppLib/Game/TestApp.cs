using Engine.Base;
using Engine.Graphic.Window;
using Engine.Events.QuantumEvents;

namespace AppLib.Game;

public class TestApp : Game<TestAppData, Window>
{
    public override TestAppData PrepareInstance()
    {
        return new TestAppData();
    }

    protected override Window CreateMainWindow()
    {
        return new Window(MetaData.WinData, MetaData.GlData, name: MetaData.Identifier.GetNameAnyway());
    }

    public override void HandleEvent(QuantumEvent e)
    {
        Console.Out.WriteLine($"event type: `{e.Type}`");
    }
}