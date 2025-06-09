using Engine.Data;
using Engine.Data.Meta;

namespace Engine.Logging;

using Data.Collections;

public class LoggersTable : WritableTale<ILogger>
{
    public LoggersTable(MetaData metaData) : base(metaData)
    {
    }

    public LoggersTable(MetaData metaData, IEnumerable<KeyValuePair<Identifier, ILogger>> data) : base(metaData, data)
    {
    }
}