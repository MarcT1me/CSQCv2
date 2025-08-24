using System.Runtime.InteropServices;

namespace Engine.Graphic.Window;

[ComImport]
[Guid("ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf")]
[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
interface ITaskbarList3
{
    void HrInit();
    void AddTab(IntPtr hwnd);
    void DeleteTab(IntPtr hwnd);
    void ActivateTab(IntPtr hwnd);
    void SetActiveAlt(IntPtr hwnd);
    void MarkFullscreenWindow(IntPtr hwnd, [MarshalAs(UnmanagedType.Bool)] bool fFullscreen);
    void SetProgressValue(IntPtr hwnd, ulong ullCompleted, ulong ullTotal);
    void SetProgressState(IntPtr hwnd, ProgressState tbpFlags);
}

public enum ProgressState
{
    NoProgress = 0,
    Indeterminate = 0x1,
    Normal = 0x2,
    Error = 0x4,
    Paused = 0x8
}

public class TaskbarProgress(IntPtr windowHandle)
{
    public void ClearProgress()
    {
        SetProgress(0, ProgressState.NoProgress);
    }

    public void SetProgress(byte value, ProgressState state = ProgressState.Normal)
    {
        var taskbarInstance = GetTaskbarInstance();
        if (taskbarInstance == null) return;

        taskbarInstance.SetProgressState(windowHandle, state);

        if (state != ProgressState.Indeterminate && state != ProgressState.NoProgress)
        {
            taskbarInstance.SetProgressValue(windowHandle, value, 100);
        }
    }

    private static ITaskbarList3? GetTaskbarInstance()
    {
        try
        {
            return (ITaskbarList3)Marshal.GetTypedObjectForIUnknown(
                Marshal.GetIUnknownForObject(new object()),
                typeof(ITaskbarList3));
        }
        catch
        {
            return null;
        }
    }
}