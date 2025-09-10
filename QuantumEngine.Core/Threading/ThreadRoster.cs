namespace Engine.Threading;

using Data.Collections;
using Data.Meta;

/// <summary>
/// Контейнер для хранения потоков
/// </summary>
public class ThreadRoster : Roster<QuantumThread>
{
    public Roster<QuantumThread> Pending => GetBranch("threadRoster-branch-pending")!;
    public Roster<QuantumThread> Worked => GetBranch("threadRoster-branch-worked")!;

    public ThreadRoster() : base(new MetaData("threadRoster"))
    {
        NewBranch(new MetaData("threadRoster-branch-pending"));
        NewBranch(new MetaData("threadRoster-branch-worked"));
    }
    
    public void Cleanup()
    {
        lock (this)
        {
            foreach (var id in Pending.Keys.ToList())
            {
                Pending.Pop(id)?.Dispose();
            }
        
            foreach (var id in Worked.Keys.ToList())
            {
                Worked.Pop(id)?.Dispose();
            }
        
            Pending.Clear();
            Worked.Clear();
        }
    }

    public void CleanupExpired()
    {
        lock (this)
        {
            CleanBranch(Pending);
            CleanBranch(Worked);
        }
    }

    private void CleanBranch(Roster<QuantumThread> branch)
    {
        foreach (var id in branch.Keys.ToList())
        {
            if (branch[id] is not { MetaData.IsExpired: true } thread) continue;
            
            thread.Join();
            thread.Dispose();
            branch.Pop(id);
        }
    }
}