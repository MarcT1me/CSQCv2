using SDL2;

namespace Engine.Events.QuantumEvents.Joy;

public class DeviceEvent(SDL.SDL_Event e) : JoyEvent(e, e.jdevice.which, e.jdevice.timestamp);