using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace Quantum.Sdk.Tasks
{
    public class QuantumAssetProcessor : Task
    {
        [Required]
        public string ProjectDir { get; set; }

        [Required]
        public string OutputPath { get; set; }

        [Required]
        public string ConfigFile { get; set; }

        public string Configuration { get; set; }

        [Output]
        public ITaskItem[] ProcessedAssets { get; set; }

        public override bool Execute()
        {
            try
            {
                Log.LogMessage(MessageImportance.High, "=== QUANTUM ASSET PROCESSOR ===");
                Log.LogMessage(MessageImportance.High, $"Project: {ProjectDir}");
                Log.LogMessage(MessageImportance.High, $"Config: {ConfigFile}");
                
                // ВРЕМЕННО: просто находим все файлы в Assets/
                var assets = new List<ITaskItem>();
                
                var assetsDir = Path.Combine(ProjectDir, "Assets");
                if (Directory.Exists(assetsDir))
                {
                    var files = Directory.GetFiles(assetsDir, "*", SearchOption.AllDirectories);
                    foreach (var file in files)
                    {
                        var item = new TaskItem(file);
                        item.SetMetadata("CopyToOutputDirectory", "PreserveNewest");
                        assets.Add(item);
                        Log.LogMessage(MessageImportance.Normal, $"Found: {file}");
                    }
                }

                ProcessedAssets = assets.ToArray();
                Log.LogMessage(MessageImportance.High, $"Total assets: {assets.Count}");
                
                return true;
            }
            catch (Exception ex)
            {
                Log.LogError($"Quantum asset processing failed: {ex.Message}");
                return false;
            }
        }
    }
}