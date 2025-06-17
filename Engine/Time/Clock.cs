using SDL2;

namespace Engine.Time;

using Defers;
using Data;
using Data.Meta;
using Data.Collections;

public sealed class Clock(ClockMeta metaData) : MetaObject<ClockMeta>(metaData)
{
    public Clock(uint tps, WritableTale<float>? speedRoster = null)
        : this(new ClockMeta(
            tps, SDL.SDL_GetTicks(), speedRoster ?? new(new MetaData())
        ))
    {
    }

    public void Tick()
    {
        foreach (Defer defer in MetaData.DeferTable.Values)
        {
            defer.Handle();
        }

        var currentCounter = SDL.SDL_GetPerformanceCounter();
        var currentFrequency = SDL.SDL_GetPerformanceFrequency();
        MetaData.Tick((double)currentCounter / currentFrequency);

        if (!(MetaData.DeltaTime < MetaData.TickDelay)) return;

        var delay = MetaData.TickDelay - MetaData.DeltaTime;
        SDL.SDL_Delay((uint)delay);
    }

    public void StartDeffer(Defer defer)
    {
        MetaData.DeferTable[defer.Id] = defer;
    }

    public void StopDeffer(Defer defer)
    {
        MetaData.DeferTable.Pop(defer.Id);
    }

    public void StopDeffer(Identifier identifier)
    {
        MetaData.DeferTable.Pop(identifier);
    }
}