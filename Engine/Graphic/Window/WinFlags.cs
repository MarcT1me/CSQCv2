namespace Engine.Graphic.Window;

[Flags]
public enum WinFlags : uint
{
    // ===========================================
    // [Group 1: Visibility States]
    // Controls initial window visibility
    // ===========================================
    Shown = 4,      // Window is immediately visible after creation
    Hidden = 8,     // Window is created in hidden state
    
    // ===========================================
    // [Group 2: Display Modes]
    // Window display/frame characteristics
    // ===========================================
    Fullscreen = 1,             // Fullscreen window (exclusive mode)
    Borderless = 16,            // Window without borders/chrome
    Resizable = 32,             // Window can be resized by user
    FullscreenDesktop = 4097,   // Fullscreen at desktop resolution
    
    // ===========================================
    // [Group 3: Window States]
    // Runtime window state management
    // ===========================================
    Minimized = 64,         // Window is minimized (iconified)
    Maximized = 128,        // Window starts maximized
    AlwaysOnTop = 32768,    // Window floats above others (z-order top)
    
    // ===========================================
    // [Group 4: Input & Focus Control]
    // Input event handling and focus behavior
    // ===========================================
    InputGrabbed = 256,     // Input exclusively captured (like mouse capture)
    InputFocus = 512,       // Window has keyboard input focus
    MouseFocus = 1024,      // Window has mouse hover focus
    MouseCapture = 16384,   // Window has global mouse capture
    
    // ===========================================
    // [Group 5: Special System Flags]
    // Special system-level behaviors
    // ===========================================
    Foreign = 2048,         // Window was created by external/foreign process
    AllowHighdpi = 8192,    // Enable High-DPI aware rendering
    SkipTaskbar = 65536,    // Window doesn't appear in taskbar/dock
    
    // ===========================================
    // [Group 6: Window Purpose Types]
    // Specialized window role hints for OS
    // ===========================================
    Utility = 131072,   // Tool palette window (docked helper)
    Tooltip = 262144,   // Floating hint window (transient popup)
    PopupMenu = 524288, // Context menu (temporary overlay)
    
    // ===========================================
    // [Group 7: Rendering Contexts]
    // Graphics API integration flags
    // ===========================================
    Vulkan = 268435456, // Window supports Vulkan rendering
    Opengl = 2          // Window supports OpenGL rendering
}