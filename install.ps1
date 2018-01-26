#!/usr/bin/env pwsh

# For the right Permission
# Set-ExecutionPolicy Bypass -Scope CurrentUser
$cmake_version = "3.10.1"
$ninja_version = "1.8.2"
$Documents = [Environment]::GetFolderPath('MyDocuments')

Write-Host "Installing Data-Analysis dependecies:" -ForegroundColor Yellow
Write-Host "- cmake"
. ".\shell_utils\pwsh\install_cmake.ps1"
Write-Host "- Python3 (snakemake)"
. ".\shell_utils\pwsh\install_python.ps1"
Write-Host "- g++ (> 4.9)"
Write-Host "- make"
. ".\shell_utils\pwsh\install_g++.ps1"
Write-Host "- Chocolatey (recommended)"
. ".\shell_utils\pwsh\install_choco.ps1"
Write-Host "- SublimeText3 (recommended)"
. ".\shell_utils\pwsh\install_sublimetext3.ps1"
Write-Host "- Ninja"
. ".\shell_utils\pwsh\install_ninja.ps1"

If( $args[1] -eq $null ) { $path2out = "toolchain" } # specify path to install programs
Else {  $path2out = $args[1] }
Write-Host "Installation path : $env:HOMEDRIVE$env:HOMEPATH\$path2out" -ForegroundColor Yellow


Push-Location
Set-Location $env:HOMEDRIVE$env:HOMEPATH
New-Item -Path $path2out -ItemType directory -Force > $null
New-Item -Path $Documents\WindowsPowerShell -ItemType directory -Force > $null
Set-Location $path2out


Write-Host "Looking for packages..."

# CMAKE Installer
Write-Host "cmake identification"
If( -Not (Get-Command cmake -ErrorAction SilentlyContinue) ){ # cmake not installed
    Write-Host "cmake not FOUND" -ForegroundColor Red
    $up_version = $cmake_version.Substring(0, $cmake_version.Length - 2)
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ install_cmake "https://cmake.org/files/v$up_version/cmake-$cmake_version-win64-x64.zip" "." $true }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n"){ Write-Host "Abort" -ForegroundColor Red }
        Else{ install_cmake "https://cmake.org/files/v$up_ver/cmake-$cmake_ver-win64-x64.zip" "." $true }
    }
}
Else{ Write-Host "cmake FOUND" -ForegroundColor Green}


# Miniconda3 Installer
Write-Host "Python3 identification"
If( -Not (Get-Command python -ErrorAction SilentlyContinue) ){ $pyver = "" }
Else {  $pyver = python -c "import sys; print(sys.version)" }# python version
If($pyver -like "*3.*" -Or $pyver -like "*Anaconda 3.*") {# right version 3. so install snakemake
    Write-Host "Python3 FOUND" -ForegroundColor Green
    Write-Host "snakemake identification"
    If( -Not (Get-Command snakemake -ErrorAction SilentlyContinue) ) {
        Write-Host "snakemake not FOUND" -ForegroundColor Red
        If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) {
            conda update conda -y
            conda config --add channels bioconda
            pip install seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
        }
        Else{
            $CONFIRM = Read-Host -Prompt "Do you want install snakemake and other dependecies? [y/n]"
            If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
            Else{
                conda update conda -y
                conda config --add channels bioconda
                pip install seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
            }
        }
    }
    Else { Write-Host "snakemake FOUND" -ForegroundColor Green}
}
ElseIf($pyver -like "*2.*" -Or $pyver -like "*Anaconda 2.*") {
    Write-Host "The Python version found is too old for snakemake" -ForegroundColor Red
    Write-Host "Please set a new version of Python as default" -ForegroundColor Red
    Write-Host "OR" -ForegroundColor Red
    Write-Host "Uninstall the current version and re-run this script for a Python3 installation" -ForegroundColor Red
}
Else{
    Write-Host "Python not FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) { install_python "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe" $true seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder}
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install snakemake and other dependecies? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
        Else { install_python "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe" $true seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder}
    }
}

# g++ Installer
Write-Host "C++ compiler identification (g++ version greater than 4.9)"
$gcc = Get-Command g++ -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Definition
If( $gcc -eq $null){$version = ""}
Else{$version = & g++ "--version"}
If( $gcc -eq $null ){ # g++ not found
    Write-Host "g++ not installed" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ install_g++ "." $true }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
        Else{ install_g++ "." $true }
    }
}
ElseIf( $version.split(' ')[6].split('.')[0] -lt 4 ){ # version too old
    Write-Host "g++ version too old for OpenMP 4" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ install_g++ "." $true }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ 
            install_g++ "." $true 
            $env:PATH = $env:PATH - ";$gcc"
            -join('$env:PATH = $env:PATH', " - `";$gcc`"") | Out-File -FilePath "$Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        }
    }
}
Else{ Write-Host "A sufficient version of g++ already installed" -ForegroundColor Green}


# Chocolatey Installer
Write-Host Chocolatey identification
$choco = Get-Command choco -ErrorAction SilentlyContinue
If( $choco -eq $null ){
    Write-Host "Chocolatey not FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) { install_choco }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ install_choco }
    }
}
Else { Write-Host Choco FOUND -ForegroundColor Green}


# Sublime Installer
Write-Host SublimeText3 identification
$sub = ((Get-ChildItem "HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall") | Where-Object { $_.GetValue( "DisplayName" ) -like "*sublime*"} | Select Name) -Or (Get-Command subl*)
If( $sub -eq $false){ # not installed
    Write-Host "SublimeText3 not installed (recomended)"
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ install_sublime "." $true }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ install_sublime "." $true }
    }
}
Else{ Write-Host SublimeText3 already installed -ForegroundColor Green}

# Ninja-build Installer
Write-Host ninja identification
If( -Not (Get-Command ninja -ErrorAction SilentlyContinue) ){ # ninja not installed
    Write-Host "ninja not FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ install_ninja "https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-win.zip" "." $true }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ install_ninja "https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-win.zip" "." $true }
    }
}
Else{ Write-Host "ninja FOUND" -ForegroundColor Green}

Pop-Location

Write-Host "Build Data-Analysis libraries with cmake-ninja" -ForegroundColor Yellow
& .\build.ps1

