using System.Globalization;
using Engine.Base;
using Engine.Graphic.Window;
using Engine.Events.QuantumEvents;
using Engine.Input.Controller;
using Engine.Logging;

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
        base.HandleEvent(e);
        if (e is Engine.Events.QuantumEvents.Joy.ButtonEvent jButton)
        {
            var joy = Joy.Get(jButton.Which);
            Logger.Debug($"{joy.JoyType}: {joy.Name}, {(JoyButtons)jButton.Button}");
        }
    }
}