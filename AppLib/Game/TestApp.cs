using Engine.Base;
using Engine.Graphic.Window;

namespace AppLib.Game;

public class TestApp : Game<TestAppData, GameWindow>
{
    public override TestAppData PrepareInstance()
    {
        return new TestAppData
        {
            Tps = 0,
            WinData = new WinData(new(1600, 900), resolutionScaling: 1f)
        };
    }

    protected override GameWindow CreateMainWindow()
    {
        var n = MetaData.Identifier.GetNameAnyway();
        return new GameWindow(MetaData.WinData, MetaData.GlData, n);
    }
}