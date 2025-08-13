using Engine.Base;
using Engine.Objects.Scene;
using Engine.Objects.SceneNode;
using Engine.Events.QuantumEvents;
using Engine.Time;
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
            WinData = new((1600, 900)),
            GlData = new() { ClipPlanes = (0.001f, 4000f) }
        };
    }

    protected override GameWindow CreateMainWindow()
    {
        return new GameWindow(
            MetaData.WinData,
            MetaData.GlData,
            MetaData.Identifier.GetNameAnyway()
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

    public override void PreUpdate(ClockMeta clockMeta)
    {
        base.PreUpdate(clockMeta);
        Scene.PreUpdate(clockMeta);
    }

    public override void Update(ClockMeta clockMeta)
    {
        base.Update(clockMeta);
        Scene.Update(clockMeta);
    }

    public override void PostUpdate(ClockMeta clockMeta)
    {
        base.PostUpdate(clockMeta);
        Scene.PostUpdate(clockMeta);
    }
}