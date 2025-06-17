namespace Engine.Objects;

[Flags]
public enum ObjectStatusFlags
{
    Active,
    Visible,
    Dynamic,
    All = Active | Visible | Dynamic
}