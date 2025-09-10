namespace QuantumCore.Base;

using Graphic.Window;

public abstract class GameData : AppData
{
    public WinData WinData { get; init; } = new(new(1600, 900));
    public GlData GlData { get; init; } = new();
}