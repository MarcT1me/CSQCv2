using SDL2;

namespace Engine.Input.Keyboard;

public struct KeySym(SDL.SDL_Keysym keysym)
{
    public Scancode Scancode = (Scancode)keysym.scancode;
    public KeyCode Sym = (KeyCode)keysym.sym;
    public KeyMod Mod = (KeyMod)keysym.mod;
    public uint Unicode = keysym.unicode;
}