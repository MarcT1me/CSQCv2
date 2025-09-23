using QuantumCore.Failures;

namespace QuantumCore.Asset.Default;

public class Configuration
{
    private readonly Dictionary<string, object> _configs = new();
    public String FileName { get; }
    public string FileContext { get; }

    public class ConfigurationError : FailureException
    {
        public ConfigurationError(Configuration conf, string message) : base(
            $"{conf}: {message}")
        {
        }

        public ConfigurationError(string message) : base(
            $"Any error occured: {message}")
        {
        }
    }

    public Configuration(string configFileName)
    {
        FileName = configFileName;
        FileContext = ReadConfigurationFile();
        CreateConfigurationData();
    }

    public ConfigurationError KeyError(string key)
    {
        return new ConfigurationError(this, $"has not have key {key}");
    }

    private string ReadConfigurationFile()
    {
        return AssetLoader.ReadText(
            new AssetFile(null!, FileName)
        );
    }

    private void CreateConfigurationData()
    {
        foreach (var line in FileContext.Split('\n'))
        {
            if (line.StartsWith("#")) continue;

            var lineContent = line.Trim().Split('=');
            if (lineContent.Length != 2) continue;

            var (key, value) = (lineContent.First(), lineContent.Last());
            if (key == "" || value == "") continue;
            _configs.Add(key.Trim(), TryParsValue(value.Trim()));
        }
    }

    private object TryParsValue(string value)
    {
        if (double.TryParse(value, out var resultDouble))
            return resultDouble;
        if (int.TryParse(value, out var resultInt))
            return resultInt;
        if (bool.TryParse(value, out var resultBool))
            return resultBool;
        return value;
    }

    public bool Contains(string name)
    {
        return _configs.ContainsKey(name);
    }

    public T Get<T>(string key)
    {
        _configs.TryGetValue(key, out var value);
        if (value == null) throw KeyError(key);
        return (T)value;
    }

    public T Get<T>(string key, T defaultValue)
    {
        _configs.TryGetValue(key, out var value);
        if (value == null) return defaultValue;
        return (T)value;
    }

    public IEnumerable<KeyValuePair<string, object>> IterConfigs()
    {
        foreach (var config in _configs)
        {
            yield return config;
        }
    }

    public override string ToString()
    {
        return $"Configuration<>(file: {FileName})";
    }
}