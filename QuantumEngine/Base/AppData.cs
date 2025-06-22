namespace Engine.Base;

using Objects;
using Configuration;
using Data.Collections;
using Data.Meta;

public abstract class AppData() : MetaData(BaseConfig.AppName)
{
    public uint Tps { get; init; }
    public Table<object> InitTable { get; init; } = new(new("EngineCore.Base.AppData-InitTable"), new());
    public WritableTale<float> SpeedRoster { get; init; } = new(new("EngineCore.Base.AppData-SpeedRoster"));
    public ObjectStatusFlags ObjectStatus = ObjectStatusFlags.All;
}