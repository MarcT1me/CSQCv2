
namespace Engine.Logging;

using Data.Meta;
using Data.Collections;

public class LoggersTable() : WritableTale<ILogger>(new MetaData("logger-table"));