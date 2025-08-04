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
            WinData = new WinData(new(1600, 900))
        };
    }

    protected override GameWindow CreateMainWindow()
    {
        return new GameWindow(MetaData.WinData, MetaData.GlData, MetaData.Identifier.GetNameAnyway());
    }
}