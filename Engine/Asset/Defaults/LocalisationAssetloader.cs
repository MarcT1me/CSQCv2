using IniParser;
using IniParser.Model;
using OpenTK.Mathematics;

namespace Engine.Asset.Defaults;

public readonly struct LocalisationData
{
    public Vector2i Range { get; }
    private readonly Dictionary<string, string> _phrases = new();

    public LocalisationData(Vector2i range, KeyDataCollection phrases)
    {
        Range = range;
        foreach (var phrase in phrases)
            AddPhrase(phrase.KeyName, phrase.Value);
    }

    public void AddPhrase(string key, string phrase)
    {
        _phrases[key] = phrase;
    }

    public string? GetPhrase(string key)
    {
        return _phrases.GetValueOrDefault(key);
    }
}

public sealed class LocalisationAssetloader : AssetLoader
{
    private static readonly FileIniDataParser IniParser = new();

    public override object LoadFile(AssetFile assetFile)
    {
        var data = IniParser.ReadFile(assetFile.GetFullPath());

        LocalisationData localisation = new(
            new Vector2i
            {
                X = int.Parse(data["char"]["first"]),
                Y = int.Parse(data["char"]["final"])
            },
            data["translation"]
        );

        return localisation;
    }

    public override AssetData CreateAsset(AssetFile assetFile, IEnumerable<AssetData>? dependencies, object content)
    {
        return new AssetData(
            assetFile.TypeName,
            content,
            identifier: assetFile.Identifier
        );
    }
}