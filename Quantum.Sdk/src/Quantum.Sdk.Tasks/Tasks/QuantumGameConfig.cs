using System.Text.Json.Serialization;

namespace Quantum.Sdk.Tasks
{
    public class QuantumConfig
    {
        [JsonPropertyName("game")]
        public GameConfig Game { get; set; } = new GameConfig();
        
        [JsonPropertyName("build")] 
        public BuildConfig Build { get; set; } = new BuildConfig();
        
        [JsonPropertyName("assets")]
        public AssetsConfig Assets { get; set; } = new AssetsConfig();
    }

    public class GameConfig
    {
        [JsonPropertyName("name")]
        public string Name { get; set; } = "Untitled";
        
        [JsonPropertyName("version")]
        public string Version { get; set; } = "1.0.0";
        
        [JsonPropertyName("company")]
        public string Company { get; set; } = "QuantumDev";
    }

    public class BuildConfig
    {
        [JsonPropertyName("icon")]
        public string Icon { get; set; } = "app.ico";
        
        [JsonPropertyName("manifest")]
        public string Manifest { get; set; } = "app.manifest";
        
        [JsonPropertyName("graphicsBackend")]
        public string GraphicsBackend { get; set; } = "DirectX12";
        
        [JsonPropertyName("enableXR")]
        public bool EnableXR { get; set; } = false;
    }

    public class AssetsConfig
    {
        [JsonPropertyName("autoImport")]
        public bool AutoImport { get; set; } = true;
        
        [JsonPropertyName("includePatterns")]
        public string[] IncludePatterns { get; set; } = new[] { "Assets/**/*" };
    }
}