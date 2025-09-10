using System.Diagnostics;
using System.Reflection;

namespace Engine.Asset;

using Data;
using Data.Meta;
using Extensions.Tracer;

/// <summary>
/// Данные загрузки ассета
/// </summary>
/// <param name="typeName">Имя типа</param>
/// <param name="path">Путь до ассета</param>
/// <param name="dependencies">Данные загрузки зависимостей</param>
/// <param name="identifier">Уникальный идентификатор ассета</param>
/// <remarks>Перед загрузкой убедитесь, что тип зарегистрирован в реестре</remarks>
public sealed class AssetFile(
    string typeName,
    string path,
    IEnumerable<AssetFile>? dependencies = null,
    Identifier? identifier = null
) : MetaData(identifier)
{
    public string TypeName { get; } = typeName;
    public string Path { get; } = path.StartsWith('/') || path.StartsWith('\\') ? path[1..] : path;
    public List<AssetFile> Dependencies { get; } = dependencies != null ? [..dependencies] : [];
    public bool IsEmbedded { get; } = path.IndexOf(':') != -1;

    /// <summary>
    /// Определяет и выдает поток данных ассета
    /// </summary>
    /// <returns>Поток данных ассета</returns>
    public Stream GetStream()
    {
        if (IsEmbedded)
        {
            // get assembly and path from path
            var (assemblyName, embedPath) = GetEmbedPath();

            // get scanned assembly
            Assembly? assembly = (Assembly?)QuantumTracer.GetScanned(assemblyName, ScanTypes.Assembly);

            // validate assembly
            if (assembly == null)
                throw new AssetError("Can't load embedded assembly");

            // getting stream
            return assembly.GetManifestResourceStream(embedPath)
                   ?? throw new AssetError($"Embedded resource ({embedPath}) not found (in {assemblyName})");
        }

        return File.OpenRead(GetFullPath());
    }

    /// <summary>
    /// Разбирает Path на данные пути Embedded ассета
    /// </summary>
    /// <returns>Сборка и путь в сборке</returns>
    public (string, string) GetEmbedPath()
    {
        var subStrings = Path.Split(":", 2);
        Debug.Assert(subStrings.Length == 2, "Path for embed file must have assembly identifier");
        return (subStrings.First(), subStrings.Last());
    }

    /// <summary>
    /// Определяет полный путь до ассета
    /// </summary>
    /// <returns>Путь до файла ассета или полный путь Embedded</returns>
    public string GetFullPath()
    {
        return !IsEmbedded ? System.IO.Path.Combine(AssetLoader.AssetDirectory, Path) : Path;
    }

    /// <summary>
    /// Определяет имя файла ассета
    /// </summary>
    /// <returns>Имя файла ассета или полный путь Embedded</returns>
    public string GetFileName() => !IsEmbedded
        ? System.IO.Path.GetFileName(Path)
        : GetEmbedName(GetEmbedPath().Item2);

    /// <summary>
    /// Выдаёт имя embedded ресурса
    /// </summary>
    /// <param name="embedPath">Логическое имя / путь в сборке</param>
    /// <returns>Имя (первый перед точкой)</returns>
    private string GetEmbedName(string embedPath) => embedPath.Split("/").Last().Split('.').First();
}