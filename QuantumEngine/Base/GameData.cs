namespace Engine.Base;

using Graphic.Window;
using Graphic.OpenGl;

public abstract class GameData : AppData
{
    public WinData WinData { get; init; } = new(new(1600, 900), flags: WinFlags.Hidden);
    public GlData GlData { get; init; } = new();
}