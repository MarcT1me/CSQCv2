namespace QuantumCore.Graphic.Font;

using Data.Meta;

public class FontData(
    string name,
    uint size,
    string family,
    FontStyle style,
    string fileDir
) : MetaData($"Font-{name}-{family}-{size}")
{
    public readonly string Name = name;
    public readonly uint Size = size;
    public uint Height { get; internal set; }
    public readonly string Family = family;
    public readonly FontStyle Style = style;

    public readonly string FontFile = Path.Combine(
        fileDir, family, style + GetFontDecoratorCharacters(style) + ".ttf"
    );

    public static string GetFontDecoratorCharacters(FontStyle style)
    {
        if (style == FontStyle.Kernel)
            return "k";
        string decorator = "";
        if (style.HasFlag(FontStyle.NStyle))
            decorator = "n";
        if (style.HasFlag(FontStyle.Bold))
            decorator += "b";
        if (style.HasFlag(FontStyle.Italic))
            decorator += "i";
        return style.HasFlag(FontStyle.NStyle) ? decorator.ToUpper() : decorator;
        ;
    }
}