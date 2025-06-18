using SDL2;

namespace Engine.Events.QuantumEvents.Controller;

public class DeviceEvent(SDL.SDL_Event e) : ControllerEvent(e, e.cdevice.which, e.cdevice.timestamp);