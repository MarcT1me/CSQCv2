namespace Engine.Threading;

using Data.Arrays;
using Data.Meta;

internal sealed class ThreadRoster : Roster<QThread>
{
    public Roster<QThread> Pending => GetBranch("threadRoster-branch-pending")!;
    public Roster<QThread> Worked => GetBranch("threadRoster-branch-worked")!;

    public ThreadRoster() : base(new MetaData("threadRoster"))
    {
        NewBranch(new MetaData("threadRoster-branch-pending"));
        NewBranch(new MetaData("threadRoster-branch-worked"));
    }

    public void CleanupExpired()
    {
        lock (this)
        {
            CleanBranch(Pending);
            CleanBranch(Worked);
        }
    }

    private void CleanBranch(Roster<QThread> branch)
    {
        foreach (var id in branch.Keys.ToList())
        {
            if (branch[id] is not { MetaData.IsExpired: true } thread) continue;
            
            thread.Join();
            thread.Dispose();
            branch[id] = null;
        }
    }
}