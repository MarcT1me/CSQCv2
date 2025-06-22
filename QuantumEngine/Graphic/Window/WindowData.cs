namespace Engine.Graphic.Window;

using Data;
using Data.Meta;
using Objects;

public class WindowData(
    WinData winData,
    GlData glData,
    // MetaData
    string? identifier
) : MetaData(Identifier.FromUncertain(identifier))
{
    public WinData WinData { get; } = winData;
    public GlData GlData { get; } = glData;
    public ObjectStatusFlags Status => ObjectStatusFlags.All;
}