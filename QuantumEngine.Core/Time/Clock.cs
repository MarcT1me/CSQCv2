using System.Diagnostics;

namespace QuantumCore.Time;

using Defers;
using Data;
using Data.Meta;
using Data.Collections;

public sealed class Clock(ClockMeta metaData) : MetaObject<ClockMeta>(metaData)
{
    private readonly Stopwatch _stopwatch = new();
    private TimeSpan _lastTime;

    public Clock(uint tps, WritableTale<float> speedRoster)
        : this(new(tps, speedRoster))
    {
    }

    public void StartClock()
    {
        _stopwatch.Start();
        _lastTime = _stopwatch.Elapsed;
        MetaData.DeltaTime = 0;
        MetaData.FrameCount = 0;
    }

    public void Tick()
    {
        ProcessDefers();
        UpdateDeltaTime();
        RegulateTickRate();
    }

    private void UpdateDeltaTime()
    {
        MetaData.FrameCount++;
        TimeSpan currentTime = _stopwatch.Elapsed;
        MetaData.DeltaTime = (currentTime - _lastTime).TotalMilliseconds;
        _lastTime = currentTime;
    }

    private void ProcessDefers()
    {
        foreach (Defer defer in MetaData.DeferTable.Values)
        {
            defer.Handle();
        }
    }

    private void RegulateTickRate()
    {
        if (MetaData.Tps == 0) return;

        while ((_stopwatch.Elapsed - _lastTime).TotalMilliseconds < MetaData.TickDelay) ;
    }

    public void StartDeffer(Defer defer) => MetaData.DeferTable[defer.Id] = defer;

    public void StopDeffer(Defer defer) => MetaData.DeferTable.Pop(defer.Id);

    public void StopDeffer(Identifier identifier) => MetaData.DeferTable.Pop(identifier);
}