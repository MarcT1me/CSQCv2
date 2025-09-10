namespace Engine.Extensions;

using Failures;

/// <summary>
/// Context manager where Constructor is Open method and Dispose is Close method (needs failure handling method)
/// </summary>
public interface IContextManager : IDisposable, IExceptionHandler;