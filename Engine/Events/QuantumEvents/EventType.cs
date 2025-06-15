namespace Engine.Events.QuantumEvents;

public enum EventType : uint
{
    FirstEvent = 0,

    Quit = 256, // 0x00000100

    WindowEvent = 512, // 0x00000200

    SyswmEvent = 513, // 0x00000201

    KeyDown = 768, // 0x00000300
    KeyUp = 769, // 0x00000301

    TextEditing = 770, // 0x00000302
    Textinput = 771, // 0x00000303

    MouseMotion = 1024, // 0x00000400
    MouseButtonDown = 1025, // 0x00000401
    MouseButtonUp = 1026, // 0x00000402
    MouseWheel = 1027, // 0x00000403

    JoyAxisMotion = 1536, // 0x00000600
    JoyBallMotion = 1537, // 0x00000601
    JoyHatMotion = 1538, // 0x00000602
    JoyButtonDown = 1539, // 0x00000603
    JoyButtonUp = 1540, // 0x00000604
    JoyDeviceAdded = 1541, // 0x00000605
    JoyDeviceRemoved = 1542, // 0x00000606

    ControllerAxisMotion = 1616, // 0x00000650
    ControllerButtonDown = 1617, // 0x00000651
    ControllerButtonUp = 1618, // 0x00000652
    ControllerDeviceAdded = 1619, // 0x00000653
    ControllerDeviceRremoved = 1620, // 0x00000654
    ControllerDeviceRremapped = 1621, // 0x00000655

    DollarGesture = 2048, // 0x00000800
    DollarRecord = 2049, // 0x00000801

    MultiGesture = 2050, // 0x00000802
    ClipboardUpdate = 2304, // 0x00000900

    DropFile = 4096, // 0x00001000
    DropText = 4097, // 0x00001001
    DropBegin = 4098, // 0x00001002
    DropComplete = 4099, // 0x00001003

    AudioDeviceAdded = 4352, // 0x00001100
    AudioDeviceRemoved = 4353, // 0x00001101

    RenderTargetsReset = 8192, // 0x00002000
    RenderDeviceReset = 8193, // 0x00002001

    UserEvent = 32768, // 0x00008000
    LastEvent = 65535 // 0x0000FFFF
}