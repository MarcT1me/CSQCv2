namespace Engine.Time.Defers;

using Data.Meta;

public abstract class Defer(DeferMeta metaData)
    : MetaObject<DeferMeta>(metaData)
{
    public void Handle()
    {
        if (!Check()) return;

        MetaData.Callback();
        if (MetaData.Disposable)
        {
            Stop();
        }
    }

    protected abstract bool Check();

    public void Start()
    {
        MetaData.AttachClock.StartDeffer(this);
    }

    public void Stop()
    {
        MetaData.AttachClock.StopDeffer(this);
    }
}