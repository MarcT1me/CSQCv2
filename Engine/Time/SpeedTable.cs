using Engine.Data;
using Engine.Data.Meta;

namespace Engine.Time;

using Data.Collections;

public class SpeedTable : WritableTale<float>
{
    public SpeedTable(MetaData metaData) : base(metaData)
    {
    }

    public SpeedTable(MetaData metaData, Dictionary<Identifier, float> data) : base(metaData, data)
    {
    }
}