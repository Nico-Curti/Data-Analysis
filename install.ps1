#!/usr/bin/env pwsh

$project = "Data-Analysis"

# For the right Permission
# Set-ExecutionPolicy Bypass -Scope CurrentUser
$cmake_version = "3.10.1"
$cmake_up_version = $cmake_version.Substring(0, $cmake_version.Length - 2)
$ninja_version = "1.8.2"


$url_cmake = "https://cmake.org/files/v$cmake_up_version/cmake-$cmake_version-win64-x64.zip"
$url_python = "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe"
$url_ninja = "https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-win.zip"
$url_sublime = "https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64.zip"
$url_choco = "https://chocolatey.org/install.ps1"
$url_gcc = "https://sourceforge.net/projects/msys2/files/Base/i686/msys2-base-i686-20161025.tar.xz"

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

$log = "install_$project.log" 

Write-Host "Looking for packages..."

# CMAKE Installer
Write-Host "cmake identification: " -NoNewLine
If( -Not (Get-Command cmake -ErrorAction SilentlyContinue) ){ # cmake not installed
    Write-Host "NOT FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ 
        Start-Transcript -Append -Path $log
        install_cmake $url_cmake "." $true
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n"){ Write-Host "Abort" -ForegroundColor Red }
        Else{ 
            Start-Transcript -Append -Path $log
            install_cmake $url_cmake "." $true
            Stop-Transcript
        }
    }
}
Else{ Write-Host "FOUND" -ForegroundColor Green}


# Miniconda3 Installer
Write-Host "(Conda)Python3 identification: " -NoNewLine
If( -Not (Get-Command python -ErrorAction SilentlyContinue) ){ $pyver = "" }
Else {  $pyver = python -c "import sys; print(sys.version)" }# python version
if(($pyver -like "*Miniconda*" -Or $pyver -like "*Anaconda*") -And ($pyver -like "*3.*")) {# right version 3. so install snakemake
    Write-Host "FOUND" -ForegroundColor Green
    Write-Host "snakemake identification: " -NoNewLine
    If( -Not (Get-Command snakemake -ErrorAction SilentlyContinue) ) {
        Write-Host "NOT FOUND" -ForegroundColor Red
        If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) {
            Start-Transcript -Append -Path $log
            conda update conda -y
            Stop-Transcript
            Start-Transcript -Append -Path $log
            conda config --add channels bioconda
            Stop-Transcript
            Start-Transcript -Append -Path $log
            pip install seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
            Stop-Transcript
        }
        Else{
            $CONFIRM = Read-Host -Prompt "Do you want install snakemake and other dependecies? [y/n]"
            If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
            Else{
                Start-Transcript -Append -Path $log
                conda update conda -y 
                Stop-Transcript
                Start-Transcript -Append -Path $log
                Stop-Transcript
                conda config --add channels bioconda
                Stop-Transcript
                Start-Transcript -Append -Path $log
                pip install seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
                Stop-Transcript
            }
        }
    }
    Else { Write-Host "FOUND" -ForegroundColor Green}
}
ElseIf(($pyver -like "*Miniconda*" -Or $pyver -like "*Anaconda*") -And ($pyver -like "*2.*")) {
    Write-Host "The Python version found is too old for snakemake" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) { 
        Start-Transcript -Append -Path $log
        install_python $url_python $true seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install a new version of Python, snakemake and other dependecies? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
        Else { 
            Start-Transcript -Append -Path $log
            install_python $url_python $true seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
            Stop-Transcript
        }
    }
}
Else{
    Write-Host "NOT FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) { 
        Start-Transcript -Append -Path $log
        install_python $url_python $true seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install snakemake and other dependecies? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
        Else { 
            Start-Transcript -Append -Path $log
            install_python $url_python $true seaborn, pandas, ipython, numpy, matplotlib, snakemake, graphviz, spyder
            Stop-Transcript
        }
    }
}

# g++ Installer
Write-Host "g++ (> 4.9) identification: " -NoNewLine
$gcc = Get-Command g++ -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Definition
If( $gcc -eq $null){$version = ""}
Else{$version = & g++ "--version"}
If( $gcc -eq $null ){ # g++ not found
    Write-Host "NOT FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ 
        Start-Transcript -Append -Path $log
        install_g++ $url_gcc "." $true
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red }
        Else{ 
            Start-Transcript -Append -Path $log
            install_g++ $url_gcc "." $true
            Stop-Transcript
        }
    }
}
ElseIf( $version.split(' ')[6].split('.')[0] -lt 4 ){ # version too old
    Write-Host "g++ version too old for OpenMP 4" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ 
        Start-Transcript -Append -Path $log
        install_g++ $url_gcc "." $true 
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ 
            Start-Transcript -Append -Path $log
            install_g++ $url_gcc "." $true
            Stop-Transcript
            $env:PATH = $env:PATH - ";$gcc"
            -join('$env:PATH = $env:PATH', " - `";$gcc`"") | Out-File -FilePath "$Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        }
    }
}
Else{ Write-Host "FOUND" -ForegroundColor Green}


# Chocolatey Installer
Write-Host "Chocolatey identification: " -NoNewLine
$choco = Get-Command choco -ErrorAction SilentlyContinue
If( $choco -eq $null ){
    Write-Host "NOT FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ) { 
        Start-Transcript -Append -Path $log
        install_choco
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ 
            Start-Transcript -Append -Path $log
            install_choco $url_choco
            Stop-Transcript
        }
    }
}
Else { Write-Host "FOUND" -ForegroundColor Green}


# Sublime Installer
Write-Host "SublimeText3 identification: " -NoNewLine
$sub = ((Get-ChildItem "HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall") | Where-Object { $_.GetValue( "DisplayName" ) -like "*sublime*"} | Select Name) -Or (Get-Command subl*)
If( $sub -eq $false){ # not installed
    Write-Host "NOT FOUND"
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ 
        Start-Transcript -Append -Path $log
        install_sublime $url_sublime "." $true
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ 
            Start-Transcript -Append -Path $log
            install_sublime $url_sublime "." $true
            Stop-Transcript
        }
    }
}
Else{ Write-Host "FOUND" -ForegroundColor Green}

# Ninja-build Installer
Write-Host "ninja-build identification: " -NoNewLine
If( -Not (Get-Command ninja -ErrorAction SilentlyContinue) ){ # ninja not installed
    Write-Host "NOT FOUND" -ForegroundColor Red
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" ){ 
        Start-Transcript -Append -Path $log
        install_ninja $url_ninja "." $true
        Stop-Transcript
    }
    Else{
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n') { Write-Host "Abort" -ForegroundColor Red}
        Else{ 
            Start-Transcript -Append -Path $log
            install_ninja $url_ninja "." $true
            Stop-Transcript
        }
    }
}
Else{ Write-Host "FOUND" -ForegroundColor Green}

Pop-Location  > $null

Write-Host "Build $project" -ForegroundColor Yellow
& .\build.ps1

