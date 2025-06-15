using SDL2;

namespace Engine.Events.QuantumEvents.Mouse;

public class MouseEvent(SDL.SDL_Event e) : WindowedQuantumEvent(e, e.button.timestamp, e.button.windowID);