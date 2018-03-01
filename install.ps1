#!/usr/bin/env pwsh

$project = "Data-Analysis"
$log = "install_$project.log" 

# For the right Permission
# Set-ExecutionPolicy Bypass -Scope CurrentUser

$Documents = [Environment]::GetFolderPath('MyDocuments')

Write-Host "Installing $project dependecies:" -ForegroundColor Yellow
Write-Host "  - cmake"
. ".\shell_utils\pwsh\install_cmake.ps1"
Write-Host "  - (Conda)Python3 and snakemake"
. ".\shell_utils\pwsh\install_python.ps1"
Write-Host "  - g++ (> 4.9)"
Write-Host "  - make"
. ".\shell_utils\pwsh\install_g++.ps1"
Write-Host "  - Chocolatey (recommended)"
. ".\shell_utils\pwsh\install_choco.ps1"
Write-Host "  - SublimeText3 (recommended)"
. ".\shell_utils\pwsh\install_sublimetext3.ps1"
Write-Host "  - Ninja"
. ".\shell_utils\pwsh\install_ninja.ps1"

If( $args[1] -eq $null ) { $path2out = "toolchain" } # specify path to install programs
Else {  $path2out = $args[1] }
Write-Host "Installation path : $env:HOMEDRIVE$env:HOMEPATH\$path2out" -ForegroundColor Green

Push-Location
Set-Location $env:HOMEDRIVE$env:HOMEPATH > $null
New-Item -Path $path2out -ItemType directory -Force > $null
New-Item -Path $Documents\WindowsPowerShell -ItemType directory -Force > $null
Set-Location $path2out

Write-Host "Looking for packages..."

# CMAKE Installer
Start-Transcript -Append -Path $log
install_cmake -add2path $true -confirm $args[0] 
Stop-Transcript
# Miniconda3 Installer
Start-Transcript -Append -Path $log
install_python -add2path $true -confirm $args[0] -modules seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
Stop-Transcript
# g++ Installer
Start-Transcript -Append -Path $log
install_g++ -add2path $true -confirm $args[0]
Stop-Transcript
# Chocolatey Installer
Start-Transcript -Append -Path $log
install_choco -confirm $args[0]
Stop-Transcript
# Sublime Installer
Start-Transcript -Append -Path $log
install_sublimetext3 -add2path $true -confirm $args[0]
Stop-Transcript
# Ninja-build Installer
Start-Transcript -Append -Path $log
install_ninja -add2path $true -confirm $args[0]
Stop-Transcript

Pop-Location > $null

Write-Host "Build $project" -ForegroundColor Yellow
& .\build.ps1

