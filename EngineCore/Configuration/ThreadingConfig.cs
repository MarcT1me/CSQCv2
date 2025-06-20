namespace Engine.Configuration;

using Failures;

/// <summary>
/// Конфигурация потоков движка
/// </summary>
public static class ThreadingConfig
{
    public static FailureLevel BaseFailureLevel = FailureLevel.Second;
    public static float? LifetimeSeconds = null;
    
    public static bool IsMultiThreadEventHanlding = false;
    public static FailureLevel IsEventCritical = FailureLevel.Second;
    public static FailureLevel IsPreUpdateCritical = FailureLevel.Second;
    public static FailureLevel IsUpdateCritical = FailureLevel.Second;
}