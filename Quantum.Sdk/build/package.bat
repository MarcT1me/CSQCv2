@echo off

echo Building Quantum.Sdk.Tasks...
dotnet build ..\src\Quantum.Sdk.Tasks\Quantum.Sdk.Tasks.csproj -c Release

echo Copying dependencies...
xcopy /Y /S "..\src\Quantum.Sdk.Tasks\bin\Release\netstandard2.0\*.dll" "..\src\Quantum.Sdk\bin\Release\TasksDependencies\"

echo Building Quantum.Sdk...
dotnet build ..\src\Quantum.Sdk\Quantum.Sdk.csproj -c Release

echo Publishing Quantum.Cli...
dotnet publish ..\src\Quantum.Cli\Quantum.Cli.csproj -c Release -r win-x64 --self-contained

echo Packaging Quantum.Sdk...
dotnet pack ..\src\Quantum.Sdk\Quantum.Sdk.csproj -c Release -o ..\dist

echo.
echo Packaging complete! 
echo Upload dist/Quantum.Sdk.{version}.nupkg to NuGet feed
echo.
pause