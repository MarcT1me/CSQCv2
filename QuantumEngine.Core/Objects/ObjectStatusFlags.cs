namespace QuantumCore.Objects;

[Flags]
public enum ObjectStatusFlags
{
    Active = 1,
    Visible = 2,
    Dynamic = 4,
    NeedsUpdate = 8,
    All = Active | Visible | Dynamic | NeedsUpdate
}