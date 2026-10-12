param([ValidateSet('Release','Debug')][string]$Configuration = 'Release')
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
$buildOutput = Join-Path $PSScriptRoot 'Debug'
New-Item -ItemType Directory -Force -Path $buildOutput | Out-Null
& "$PSScriptRoot\premake\premake5.exe" vs2019
if ($LASTEXITCODE -ne 0) { throw 'Premake failed' }
foreach ($project in @('Fyredon','Client')) {
    $projectFile = Join-Path $PSScriptRoot "$project\$project.vcxproj"
    [xml]$xml = Get-Content -LiteralPath $projectFile -Raw
    $ns = New-Object System.Xml.XmlNamespaceManager($xml.NameTable)
    $ns.AddNamespace('m','http://schemas.microsoft.com/developer/msbuild/2003')
    foreach ($node in $xml.SelectNodes('//m:OutDir',$ns)) { $node.InnerText = '..\Debug\' }
    foreach ($node in $xml.SelectNodes('//m:IntDir',$ns)) { $node.InnerText = "..\Debug\obj\$Configuration\$project\" }
    foreach ($node in $xml.SelectNodes('//m:PostBuildEvent/m:Command',$ns)) { $node.InnerText = '' }
    $xml.Save($projectFile)
}
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vs = & $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (!$vs) { throw 'Visual Studio C++ tools not found' }
# Normalize inherited environment key casing for MSBuild's .NET Framework runtime.
$start = New-Object System.Diagnostics.ProcessStartInfo
$start.FileName = Join-Path $vs 'MSBuild\Current\Bin\MSBuild.exe'
$start.Arguments = "Fyredon.sln /m:1 /nr:false /p:Configuration=$Configuration /p:Platform=x64 /v:minimal /nologo"
$start.WorkingDirectory = $PSScriptRoot
$start.UseShellExecute = $false
$start.RedirectStandardOutput = $true
$start.RedirectStandardError = $true
$start.EnvironmentVariables.Clear()
Get-ChildItem Env: | ForEach-Object { $start.EnvironmentVariables[$_.Name.ToUpperInvariant()] = $_.Value }
$process = [System.Diagnostics.Process]::Start($start)
$stdout = $process.StandardOutput.ReadToEndAsync()
$stderr = $process.StandardError.ReadToEndAsync()
$process.WaitForExit()
[IO.File]::WriteAllText((Join-Path $buildOutput 'build.log'),$stdout.Result + $stderr.Result)
if ($process.ExitCode -ne 0) { Get-Content "$buildOutput\build.log" -Tail 30; throw 'Build failed; see Debug\build.log' }
Copy-Item '.\Extern\Binary\FBX\libfbxsdk.dll' $buildOutput -Force
foreach ($folder in @('script','dir_test')) { Copy-Item ".\Client\$folder" $buildOutput -Recurse -Force }
if (!(Test-Path "$buildOutput\imgui.ini")) { Copy-Item '.\Client\imgui.ini' $buildOutput }
Write-Output "Built $Configuration x64: $buildOutput\Client.exe"
