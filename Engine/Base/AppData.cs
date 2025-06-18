namespace Engine.Base;

using Objects;
using Configuration;
using Data.Collections;
using Data.Meta;

public abstract class AppData() : MetaData(BaseConfig.AppName)
{
    public uint Tps { get; init; } = 60;
    public Table<object> InitTable { get; init; } = new(new("Engine.Base.AppData-InitTable"), new());
    public WritableTale<float> SpeedRoster { get; init; } = new(new("Engine.Base.AppData-SpeedRoster"));
    public ObjectStatusFlags ObjectStatus = ObjectStatusFlags.All;
}