using Microsoft.Toolkit.Uwp.Notifications;

namespace QuantumCore.UI;

public class ToastNotification
{
    public readonly ToastContentBuilder Builder = new();

    protected ToastNotification(DateTime? timeStamp = null)
    {
        // ReSharper disable once VirtualMemberCallInConstructor
        Build(Builder);
        if (timeStamp.HasValue) Builder.AddCustomTimeStamp(timeStamp.Value);
    }

    public virtual void Build(ToastContentBuilder builder)
    {
    }

    public virtual void Show()
    {
        Builder.Show();
    }
}