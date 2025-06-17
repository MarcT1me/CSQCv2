namespace Engine.Time;

using Data.Meta;
using Data.Collections;
using Defers;

public sealed class ClockMeta : TimedMetaData
{
    private uint _tickRate;

    public WritableTale<float> SpeedTable { get; init; }
    public WritableTale<Defer> DeferTable { get; init; }

    public double TickDelay { get; private set; }
    public double DeltaTime { get; private set; }
    public int FrameCount { get; private set; }
    public double CurrentTime { get; private set; }

    public uint Tps
    {
        get => _tickRate;
        set
        {
            if (value == 0)
            {
                TickDelay = 0;
            }
            else
            {
                _tickRate = value;
                TickDelay = 1000.0d / value;
            }
        }
    }

    public double CurrentTps => 1.0d / DeltaTime;

    public ClockMeta(uint tickRate, double currentTime, WritableTale<float> speedTable)
        : base($"ClockData<{currentTime}, {tickRate}>")
    {
        Tps = tickRate;
        CurrentTime = currentTime;
        SpeedTable = speedTable;
        DeferTable = new (new MetaData());
    }

    public void Tick(double currentTime)
    {
        CurrentTime = currentTime;
        DeltaTime = currentTime - CurrentTime;
        FrameCount++;
    }
}