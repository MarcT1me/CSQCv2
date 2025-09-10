namespace QuantumCore.Time;

using Data.Meta;
using Data.Collections;
using Defers;

public sealed class ClockMeta : TimedMetaData
{
    private uint _tickRate;

    public WritableTale<float> SpeedTable { get; init; }
    public WritableTale<Defer> DeferTable { get; init; }

    public double TickDelay { get; private set; }
    public double DeltaTime { get; internal set; }
    public int FrameCount { get; internal set; }

    public uint Tps
    {
        get => _tickRate;
        set
        {
            _tickRate = value;
            TickDelay = value == 0 ? 0 : 1000.0d / value;
        }
    }

    public double ActualTps => 1000 / DeltaTime;

    public ClockMeta(uint tickRate, WritableTale<float> speedTable)
        : base($"ClockDat<{tickRate}>")
    {
        Tps = tickRate;
        SpeedTable = speedTable;
        DeferTable = new (new MetaData());
    }
}