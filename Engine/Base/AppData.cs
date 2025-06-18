namespace Engine.Base;

using Configuration;
using Data;
using Data.Collections;
using Data.Meta;
using Time;

public abstract class AppData(ClockMeta clockMeta, Dictionary<Identifier, object>? initData = null) 
    : MetaData(BaseConfig.AppName)
{
    public ClockMeta ClockMeta { get; } = clockMeta;
    public Table<object> InitTable { get; } = new(new("AppData.InitTable-Table"), initData ?? new());
}