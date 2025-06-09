namespace Engine.Logging;

using Data.Meta;

/// <summary>
/// Данные класса для логирования
/// </summary>
/// <param name="filePath">Путь до файла</param>
/// <param name="logFormat">Формат логирования</param>
/// <param name="isActive">Статус</param>
/// <param name="timeDuration">Время жизни файла</param>
public class LoggerMeta(
    string? filePath = null,
    string? logFormat = null,
    bool isActive = true,
    float? timeDuration = null
) : TimedMetaData(
    lifetimeSeconds: timeDuration
)
{
    public bool IsActive = isActive;
    public LoggerFile? File = LoggerFile.GetOrCreate(filePath);
    public readonly string LogFormat = logFormat ?? "{0} | {1} | {2} | {3}";
}