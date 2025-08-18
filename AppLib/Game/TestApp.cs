using Engine.Asset;
using Engine.Asset.Image;
using Engine.Base;
using Engine.Objects.Scene;
using Engine.Objects.SceneNode;
using Engine.Events.QuantumEvents;
using Engine.Time;
using MirageAPI;

namespace AppLib.Game;

using GameType = Game<TestAppData, GameWindow>;

public class TestApp : GameType
{
    public Scene<SceneNodeData> Scene;
    public new static TestApp Instance => (TestApp)GameType.Instance;

    public override TestAppData PrepareInstance()
    {
        return new TestAppData();
    }

    protected override GameWindow CreateMainWindow()
    {
        var winName = MetaData.Identifier.GetNameAnyway();

        return new GameWindow(
            MetaData.WinData,
            MetaData.GlData,
            winName,
            icon: new IconInfo(AssetLoader.AssetDirectory + "/Logo.ico")
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