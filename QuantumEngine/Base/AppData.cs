namespace Engine.Base;

using Objects;
using Data.Collections;
using Data.Meta;

public abstract class AppData() : MetaData(EngineCore.Core.Configuration.Get<string>("app.default.name"))
{
    public uint Tps { get; init; }
    public Table<object> InitTable { get; init; } = new(new("QuantumEngine.Core.Base.AppData-InitTable"), new());
    public WritableTale<float> SpeedRoster { get; init; } = new(new("QuantumEngine.Core.Base.AppData-SpeedRoster"));
    public ObjectStatusFlags ObjectStatus = ObjectStatusFlags.All;
}