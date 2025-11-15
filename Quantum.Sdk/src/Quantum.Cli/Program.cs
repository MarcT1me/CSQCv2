using System.Text.Json;
using Quantum.Sdk.Tasks;

namespace Quantum.Cli;

static class Program
{
    static void Main(string[] args)
    {
        if (args.Length == 0)
        {
            ShowHelp();
            return;
        }

        try
        {
            switch (args[0].ToLower())
            {
                case "new":
                    CreateNewProject(args);
                    break;
                case "build":
                    BuildProject(args);
                    break;
                case "publish":
                    PublishProject(args);
                    break;
                case "init":
                    InitEngine(args);
                    break;
                case "templates":
                    ListTemplates();
                    break;
                default:
                    ShowHelp();
                    break;
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
            Environment.Exit(1);
        }
    }

    static void CreateNewProject(string[] args)
    {
        if (args.Length < 2)
        {
            Console.WriteLine("Usage: quantum new <project-name> [template]");
            Console.WriteLine("Available templates: console, desktop, vr");
            return;
        }

        var projectName = args[1];
        var template = args.Length > 2 ? args[2] : "desktop";

        // Проверяем установлен ли движок
        var enginePath = Environment.GetEnvironmentVariable("QUANTUM_ENGINE_PATH");
        if (string.IsNullOrEmpty(enginePath))
        {
            Console.WriteLine("Quantum Engine not found. Please run 'quantum init <engine-path>' first.");
            return;
        }

        Console.WriteLine($"Creating new Quantum project: {projectName} with template: {template}");

        // Создание структуры папок
        Directory.CreateDirectory(projectName);
        Directory.CreateDirectory(Path.Combine(projectName, "Assets"));
        Directory.CreateDirectory(Path.Combine(projectName, "Source"));

        // Создание конфига в зависимости от шаблона
        var config = CreateConfigForTemplate(projectName, template);
        var configJson = JsonSerializer.Serialize(config, new JsonSerializerOptions { WriteIndented = true });
        File.WriteAllText(Path.Combine(projectName, "Quantum.gameconfig"), configJson);

        // Создание .csproj файла
        var csprojContent = CreateProjectFile(projectName, template);
        File.WriteAllText(Path.Combine(projectName, $"{projectName}.csproj"), csprojContent);

        // Создание основного файла игры
        var gameContent = CreateGameClass(projectName, template);
        File.WriteAllText(Path.Combine(projectName, "Source", "Game.cs"), gameContent);

        // Копирование шаблонных ассетов
        CopyTemplateAssets(projectName, template, enginePath);

        Console.WriteLine($"Project {projectName} created successfully!");
        Console.WriteLine($"Next steps:");
        Console.WriteLine($"  cd {projectName}");
        Console.WriteLine($"  quantum build");
        Console.WriteLine($"  dotnet run");
    }

    static void InitEngine(string[] args)
    {
        if (args.Length < 2)
        {
            Console.WriteLine("Usage: quantum init <engine-path>");
            return;
        }

        var enginePath = args[1];
        if (!Directory.Exists(enginePath))
        {
            Console.WriteLine($"Engine path does not exist: {enginePath}");
            return;
        }

        // Устанавливаем переменную среды
        Environment.SetEnvironmentVariable("QUANTUM_ENGINE_PATH", enginePath, EnvironmentVariableTarget.User);
        
        // Создаем конфиг для SDK
        var sdkConfig = new
        {
            EnginePath = enginePath,
            Version = "1.0.0",
            LastUpdated = DateTime.Now
        };
        
        var configPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.UserProfile), 
                                    ".quantum", "sdk-config.json");
        Directory.CreateDirectory(Path.GetDirectoryName(configPath));
        File.WriteAllText(configPath, JsonSerializer.Serialize(sdkConfig, new JsonSerializerOptions { WriteIndented = true }));

        Console.WriteLine($"Quantum Engine initialized at: {enginePath}");
    }

    static QuantumConfig CreateConfigForTemplate(string projectName, string template)
    {
        return template.ToLower() switch
        {
            "vr" => new QuantumConfig
            {
                Game = new GameConfig { Name = projectName, Version = "1.0.0" },
                Build = new BuildConfig { GraphicsBackend = "DirectX12", EnableXR = true },
                Assets = new AssetsConfig { AutoImport = true, IncludePatterns = new[] { "Assets/**/*" } }
            },
            "console" => new QuantumConfig
            {
                Game = new GameConfig { Name = projectName, Version = "1.0.0" },
                Build = new BuildConfig { GraphicsBackend = "None", EnableXR = false },
                Assets = new AssetsConfig { AutoImport = true, IncludePatterns = new[] { "Assets/**/*" } }
            },
            _ => new QuantumConfig // desktop
            {
                Game = new GameConfig { Name = projectName, Version = "1.0.0" },
                Build = new BuildConfig { GraphicsBackend = "DirectX12", EnableXR = false },
                Assets = new AssetsConfig { AutoImport = true, IncludePatterns = new[] { "Assets/**/*" } }
            }
        };
    }

    static string CreateProjectFile(string projectName, string template)
    {
        return $$"""
                <Project Sdk="Quantum.Sdk/0.2.0">
                  <PropertyGroup>
                    <OutputType>Exe</OutputType>
                    <TargetFramework>net9.0-windows10.0.22621.0</TargetFramework>
                    <QuantumTemplate>{{template}}</QuantumTemplate>
                  </PropertyGroup>
                </Project>
                """;
    }

    static string CreateGameClass(string projectName, string template)
    {
        return template.ToLower() switch
        {
            "console" => $$"""
                           using System;
                           using QuantumEngine;

                           [QuantumGame(Title = "{{projectName}}")]
                           public class Game : QGame
                           {
                               public override void Initialize()
                               {
                                   Console.WriteLine("Hello from Quantum Console Game!");
                               }

                               public override void Update(float deltaTime)
                               {
                                   // Console game logic here
                               }
                           }
                           """,
            "vr" => $$"""
                      using QuantumEngine;

                      [QuantumGame(Title = "{{projectName}}", EnableVR = true)]
                      public class Game : QGame
                      {
                          public override void Initialize()
                          {
                              // VR initialization
                          }

                          public override void Update(float deltaTime)
                          {
                              // VR game logic
                          }
                      }
                      """,
            _ => $$"""
                   using QuantumEngine;

                   [QuantumGame(Title = "{{projectName}}")]
                   public class Game : QGame
                   {
                       public override void Initialize()
                       {
                           // Your game initialization code here
                       }

                       public override void Update(float deltaTime)
                       {
                           // Your game logic here
                       }
                   }
                   """
        };
    }

    static void CopyTemplateAssets(string projectName, string template, string enginePath)
    {
        var templatePath = Path.Combine(enginePath, "Templates", template);
        if (Directory.Exists(templatePath))
        {
            CopyDirectory(templatePath, Path.Combine(projectName, "Assets"), true);
        }
    }

    static void BuildProject(string[] args)
    {
        var additionalArgs = args.Length > 1 ? string.Join(" ", args.Skip(1)) : "";
        var process = System.Diagnostics.Process.Start("dotnet", $"build {additionalArgs}");
        process.WaitForExit();
    }

    static void PublishProject(string[] args)
    {
        var additionalArgs = args.Length > 1 ? string.Join(" ", args.Skip(1)) : "-c Release -r win-x64";
        var process = System.Diagnostics.Process.Start("dotnet", $"publish {additionalArgs}");
        process.WaitForExit();
    }

    static void ListTemplates()
    {
        Console.WriteLine("Available templates:");
        Console.WriteLine("  console - Console application (no graphics)");
        Console.WriteLine("  desktop - Desktop game with graphics");
        Console.WriteLine("  vr      - VR game with OpenXR support");
    }

    static void ShowHelp()
    {
        Console.WriteLine("Quantum CLI Tool");
        Console.WriteLine("Usage:");
        Console.WriteLine("  quantum init <engine-path>    - Initialize Quantum Engine");
        Console.WriteLine("  quantum new <project-name> [template] - Create new project");
        Console.WriteLine("  quantum build [args]          - Build project");
        Console.WriteLine("  quantum publish [args]        - Publish project");
        Console.WriteLine("  quantum templates             - List available templates");
        Console.WriteLine("");
        Console.WriteLine("Examples:");
        Console.WriteLine("  quantum init D:\\Engines\\QuantumEngine");
        Console.WriteLine("  quantum new MyGame vr");
        Console.WriteLine("  quantum build --configuration Release");
    }

    static void CopyDirectory(string sourceDir, string destinationDir, bool recursive)
    {
        var dir = new DirectoryInfo(sourceDir);
        if (!dir.Exists)
            return;

        Directory.CreateDirectory(destinationDir);

        foreach (FileInfo file in dir.GetFiles())
        {
            string targetFilePath = Path.Combine(destinationDir, file.Name);
            file.CopyTo(targetFilePath, true);
        }

        if (recursive)
        {
            foreach (DirectoryInfo subDir in dir.GetDirectories())
            {
                string newDestinationDir = Path.Combine(destinationDir, subDir.Name);
                CopyDirectory(subDir.FullName, newDestinationDir, true);
            }
        }
    }
}