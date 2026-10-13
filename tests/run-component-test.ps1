param([ValidateSet('map_regression','reset_regression','dance_scene_regression','preview_regression')][string]$Test='map_regression')
$ErrorActionPreference='Stop'
Set-Location (Split-Path $PSScriptRoot)
$vswhere=Join-Path ([Environment]::GetFolderPath('ProgramFilesX86')) 'Microsoft Visual Studio/Installer/vswhere.exe'
$vs=& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
Get-ChildItem Debug/obj/Release/Fyredon -Filter *.obj | ForEach-Object {'"Debug\obj\Release\Fyredon\'+$_.Name+'"'} | Set-Content Debug/test-objects.rsp -Encoding ascii
@"
@call "$vs\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /c /EHsc /MD /std:c++17 /DFD_BUILD_DLL /DGLEW_STATIC /DFBXSDK_SHARED /IFyredon\Include /IExtern\Include /IExtern\Include\FBX /IExtern\Include\dearimgui tests\$Test.cpp /FoDebug\$Test.obj
@if errorlevel 1 exit /b 1
link /nologo /OUT:Debug\$Test.exe Debug\$Test.obj @Debug\test-objects.rsp /LIBPATH:Extern\Binary\GLEW /LIBPATH:Extern\Binary\GLFW /LIBPATH:Extern\Binary\FBX glew32s.lib glfw3.lib opengl32.lib libfbxsdk.lib user32.lib gdi32.lib shell32.lib
"@ | Set-Content Debug/build-component-test.cmd
& cmd.exe /c Debug\build-component-test.cmd
if($LASTEXITCODE -ne 0){throw 'Test build failed'}
Push-Location Debug
try {
 & ".\$Test.exe" > "$Test.log" 2>&1
 $result=$LASTEXITCODE
 Get-Content "$Test.log" -Tail 8
 if($result -ne 0){throw "Test failed: $result"}
} finally {Pop-Location}
