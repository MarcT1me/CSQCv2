namespace Engine.Objects;

[Flags]
public enum ObjectStatusFlags
{
    Active = 1,
    Visible = 2,
    Dynamic = 4,
    All = Active | Visible | Dynamic
}