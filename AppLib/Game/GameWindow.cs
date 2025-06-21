using Engine.Graphic.Window;

namespace AppLib.Game;

public class GameWindow(WinData winData, GlData? glData = null, string? name = null)
    : Window(winData, glData, name);