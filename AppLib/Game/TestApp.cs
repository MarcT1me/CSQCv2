using Engine.Base;

namespace AppLib.Game;

public class TestApp : Game<TestAppData, GameWindow>
{
    public override TestAppData PrepareInstance()
    {
        return new TestAppData
        {
            Tps = 0
        };
    }

    protected override GameWindow CreateMainWindow()
    {
        var n = MetaData.Identifier.GetNameAnyway();
        return new GameWindow(MetaData.WinData, MetaData.GlData, n);
    }
}