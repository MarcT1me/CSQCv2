namespace Engine.Asset;

using Data;
using Data.Meta;

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
    public string Path { get; } = path;
    public List<AssetFile> Dependencies { get; } = dependencies != null ? [..dependencies] : [];

    /// <summary>
    /// Полный путь до ассета в файловой системе
    /// </summary>
    /// <returns>Путь до файла ассета</returns>
    public string GetFullPath()
    {
        return System.IO.Path.Combine(
            AssetLoader.AssetDirectory,
            Path
        );
    }
}