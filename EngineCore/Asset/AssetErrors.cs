namespace Engine.Asset;

using Failures;

public class AssetError : FailureException
{
    public AssetError(string message) : base(message)
    {
    }

    public AssetError(string message, Exception innerException) : base(message, innerException)
    {
    }
}

public sealed class CyclicDependencyError : AssetError
{
    public CyclicDependencyError(AssetFile file
    ) : base($"Cyclic dependency detected: {file.Path}")
    {
    }

    public CyclicDependencyError(AssetFile file, Exception innerException
    ) : base($"Cyclic dependency detected: {file.Path}", innerException)
    {
    }
}

public sealed class InvalidAssetTypeError : AssetError
{
    public InvalidAssetTypeError(string typeName
    ) : base($"Asset type {typeName} not found")
    {
    }

    public InvalidAssetTypeError(string typeName, Exception innerException
    ) : base($"Asset type {typeName} not found", innerException)
    {
    }
}
