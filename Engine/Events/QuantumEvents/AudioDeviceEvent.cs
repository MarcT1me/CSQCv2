using SDL2;

namespace Engine.Events.QuantumEvents;

public class AudioDeviceEvent(SDL.SDL_Event e) : QuantumEvent(e, e.adevice.timestamp)
{
    public uint Which = e.adevice.which;
    public byte Iscapture = e.adevice.iscapture;
}