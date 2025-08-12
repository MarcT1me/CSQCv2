using Engine.Base;
using Engine.Graphic.Window;
using Engine.Objects.Scene;
using Engine.Objects.SceneNode;
using Engine.Events.QuantumEvents;
using MirageAPI.Window;

namespace AppLib.Game;

using GameType = Game<TestAppData, GameWindow>;

public class TestApp : GameType
{
    public Scene<SceneNodeData> Scene;
    public new static TestApp Instance => (TestApp)GameType.Instance;

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
        return new GameWindow(
            MetaData.WinData,
            MetaData.GlData,
            MetaData.Identifier.GetNameAnyway(),
            monitor: NativeDisplay.GetAllMonitors().Last()
        );
    }

    public TestApp()
    {
        Scene = new(new());
        Scene.AddChild(MainWindow.Camera);
    }

    public override void HandleEvent(QuantumEvent e)
    {
        base.HandleEvent(e);
        Scene.HandleEvent(e);
    }

    public override void PreUpdate()
    {
        base.PreUpdate();
        Scene.PreUpdate();
    }

    public override void Update()
    {
        base.Update();
        Scene.Update();
    }

    public override void PostUpdate()
    {
        base.PostUpdate();
        Scene.PostUpdate();
    }
}