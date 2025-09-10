namespace QuantumCore.Logging;

using Data;

/// <summary>
/// Базовый интерфейс Логгера
/// </summary>
public interface ILogger
{
    Identifier Id { get; }

    /// <summary>
    ///  Базовый класс для вывода сообщения
    /// </summary>
    /// <param name="level">Уровень сообщения</param>
    /// <param name="message">Само сообщение</param>
    void Log(LogLevel level, string message);
    
    /// <summary>
    /// Вывод сообщения без форматирования, заданного в логере
    /// </summary>
    /// <param name="message">Сообщение</param>
    void LogWithoutFormat(string message);
}