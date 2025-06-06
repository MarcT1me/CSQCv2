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
    public InvalidAssetTypeError(AssetFile file
    ) : base($"Asset type {file.TypeName} not found")
    {
    }

    public InvalidAssetTypeError(AssetFile file, Exception innerException
    ) : base($"Asset type {file.TypeName} not found", innerException)
    {
    }
}
