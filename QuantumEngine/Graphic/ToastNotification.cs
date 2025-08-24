using Microsoft.Toolkit.Uwp.Notifications;

namespace Engine.Graphic;

public abstract class ToastNotification
{
    private readonly ToastContentBuilder _builder = new();

    protected ToastNotification(DateTime? timeStamp = null)
    {
        // ReSharper disable once VirtualMemberCallInConstructor
        Build(_builder);
        if (timeStamp.HasValue) _builder.AddCustomTimeStamp(timeStamp.Value);
    }

    public abstract void Build(ToastContentBuilder builder);

    public virtual void Show()
    {
        _builder.Show();
    }
}