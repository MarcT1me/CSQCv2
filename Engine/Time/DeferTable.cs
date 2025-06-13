namespace Engine.Time;

using Defers;
using Data.Meta;
using Data.Collections;

public class DeferTable(MetaData metaData) : WritableTale<Defer>(metaData)
{
}