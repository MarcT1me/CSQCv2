using Engine.Graphic.OpenGl;
using Engine.Objects;

namespace Engine.Graphic.Window;

using Data.Meta;

public class WindowData(
    WinData winData,
    GlData glData,
    // MetaData
    string? identifier
) : MetaData(identifier)
{
    public WinData WinData { get; } = winData;
    public GlData GlData { get; } = glData;
    public ObjectStatusFlags Status => ObjectStatusFlags.All;
}