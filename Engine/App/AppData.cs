namespace Engine.App;

using Data.Meta;
using Time;

public abstract class AppData(ClockMeta clockMeta) : MetaData
{
    public ClockMeta ClockMeta { get; } = clockMeta;
}