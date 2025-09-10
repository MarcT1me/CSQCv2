using System.Runtime.InteropServices;
using Windows.Foundation.Metadata;

namespace QuantumCore.UI;

[ComImport]
[System.Runtime.InteropServices.Guid("ea1afb91-9e28-4b86-90e9-9e9f8a5eefaf")]
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

[ComImport]
[System.Runtime.InteropServices.Guid("56FDF344-FD6D-11d0-958A-006097C9A090")]
[ClassInterface(ClassInterfaceType.None)]
class TaskbarInstance { }

public enum ProgressState
{
    NoProgress = 0,
    Indeterminate = 0x1,
    Normal = 0x2,
    Error = 0x4,
    Paused = 0x8
}

[Deprecated("Not ready for use", DeprecationType.Deprecate, 21)]

public class TaskbarProgress
{
    private readonly IntPtr _windowHandle;
    private readonly ITaskbarList3? _taskbarInstance;
    private uint _value;
    public uint Total;

    public TaskbarProgress(IntPtr windowHandle, uint initialValue = 0, uint total = 100)
    {
        _windowHandle = windowHandle;
        _value = initialValue;
        Total = total;

        try
        {
            // ReSharper disable once SuspiciousTypeConversion.Global
            _taskbarInstance = (ITaskbarList3)new TaskbarInstance();
            _taskbarInstance.HrInit();
        }
        catch
        {
            _taskbarInstance = null;
        }

        SetProgress(initialValue, total);
    }

    public void ClearProgress()
    {
        SetProgress(0, ProgressState.NoProgress);
    }

    public void SetProgress(uint value, uint total, ProgressState state = ProgressState.Normal)
    {
        Total = total;
        SetProgress(value, state);
    }

    public void SetProgress(uint value, ProgressState state = ProgressState.Normal)
    {
        if (_taskbarInstance == null) return;

        _taskbarInstance.SetProgressState(_windowHandle, state);

        if (state != ProgressState.Indeterminate && state != ProgressState.NoProgress)
        {
            _taskbarInstance.SetProgressValue(_windowHandle, value, Total);
            _value = value;
        }
        else
        {
            _value = 0;
        }
    }

    public double GetProgressPercents() => (double)_value / Total * 100;
}