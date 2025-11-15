#!/bin/bash

# Сборка Tasks assembly
dotnet build src/Quantum.Sdk/Quantum.Sdk.csproj -c Release

# Сборка CLI
dotnet publish src/Quantum.Cli/Quantum.Cli.csproj -c Release -r win-x64 --self-contained

# Упаковка SDK
dotnet pack src/Quantum.Sdk/Quantum.Sdk.csproj -c Release -o ../../dist

echo "Packaging complete! Upload dist/Quantum.Sdk.0.2.1.nupkg to NuGet feed"