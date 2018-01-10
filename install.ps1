# For the right Permission
# Set-ExecutionPolicy Bypass -Scope Process -Force;

# Chocolatey installation (recomended)
Write-Host "Installation of Chocolatey"
If( -Not (Get-Command choco -ErrorAction SilentlyContinue) )
{
    Write-Host "Chocolatey not installed"
    $CONFIRM = Read-Host -Prompt "Do you want install it (recomended)? [y/n]"
    if($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
    {
        Write-Host "Abort Chocolatey installation"
    }
    Else
    {
        Write-Host "Chocolatey will be install in C:\ProgramData\chocoportable"
        # Set directory for installation - Chocolatey does not lock
        # down the directory if not the default
        $InstallDir='C:\ProgramData\chocoportable'
        $env:ChocolateyInstall="$InstallDir"
        # If your PowerShell Execution policy is restrictive, you may
        # not be able to get around that. Try setting your session to
        # Bypass.
        Set-ExecutionPolicy Bypass
        # All install options - offline, proxy, etc at
        # https://chocolatey.org/install
        iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
    }
}
Else
{
    Write-Host "Choco already installed"
}

#Set-ExecutionPolicy Bypass -Scope Process -Force; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
# SublimeText3 installation (recomended)
#Start-BitsTransfer -Source https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64%20Setup.exe

function Install($program, $url)
{
    Write-Host $program + " installation"
    If( -Not (Get-Command $program -ErrorAction SilentlyContinue) )
    {
        Write-Host $program + " not installed"
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        if($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host "Abort " + $program + " installation"
        }
        Else
        {
            Write-Host "Download " + $program + " from " + $url
            $token = @($url.split('/'))
            $out_dir = $token[$token.Count - 1]
            $out = @($out_dir.split("-"))
            Start-BitsTransfer -Source $url
            if($out_dir -like "*.tar.gz*" -or $out_dir -like "*.zip")
            {
                Write-Host "unzip " + $out_dir
                Expand-Archive $out_dir -DestinationPath 
            }
            Else
            {
                Write-Host "already unzip"
            }
            Move-Item $out_dir "C:\Program Files\" + $out[0]
            $oldpath = (Get-ItemProperty -Path "Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment" -Name PATH).path
            $newpath = "$path;C:\Program Files\" + $out[0] + "\bin\"
            Set-ItemProperty -Path "Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment" -Name PATH -Value $newPath
            Remove-Item $out_dir -Force -Recurse -ErrorAction SilentlyContinue
        }
    }
    Else
    {
        Write-Host $program + " already installed"
    }
}

#Set-Variable -Name "OMP_CANCELLATION" -Value True -Option constant -Scope global -Description "Set cancellation of the parallel loops"
Install "cmake" "https://cmake.org/files/v3.10/cmake-3.10.1-win64-x64.msi"
Install "ninja" "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-win.zip"
Install "sublime" "https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64.zip"

# Miniconda3 Installer
Write-Host "Python3 is required for snakemake use."
$pyver = python -c "import sys; print(sys.version)" # python version
If($pyver -like "*3.*" -Or $pyver -like "*Anaconda 3.*") # right version 3. so install snakemake
{
    Write-Host "Python3 is already install."
    $CONFIRM = Read-Host -Prompt "Do you want install snakemake and other dependecies? [y/n]"
    If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n')
    {
        Write-Host "Abort"
        exit
    }
    conda update conda -y
    conda config --add channels bioconda
    pip install seaborn pandas ipython numpy matplotlib snakemake
}
ElseIf($pyver -like "*2.*" -Or $pyver -like "*Anaconda 2.*")
{
    Write-Host "The Python version found is too old for snakemake."
    Write-Host "Please set a new version of Python as default"
    Write-Host "OR"
    Write-Host "Uninstall the current version and re-run this script for a Python3 installation."
}
Else
{
    Write-Host "No Python version found."
    $CONFIRM = Read-Host -Prompt "Do you want install Python3 and other dependecies? [y/n]"
    If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n')
    {
        Write-Host "Abort"
        exit
    }
    Write-Host "Download Miniconda3 from https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe"
    Start-BitsTransfer -Source "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe"

    Write-Host "Miniconda3 will be install and it will set as default python."
    start /wait "" Miniconda3-latest-Windows-x86_64.exe /InstallationType=JustMe /RegisterPython=1 /S /D=%UserProfile%\Miniconda3
    conda update conda -y
    conda config --add channels bioconda
    pip install seaborn pandas ipython numpy matplotlib snakemake scikit-learn
}

## install gcc new version for OpenMP 4. support
#Start-BitsTransfer -Source https://sourceforge.net/projects/msys2/files/Base/i686/msys2-base-i686-20161025.tar.xz
#cmake -E tar zxf msys2-base-i686-20161025.tar.xz
#Remove-Item msys2-base-i686-20161025.tar.xz -Force -Recurse -ErrorAction SilentlyContinue
## TODO add to path
#Set-ItemProperty -Path "Registry::HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\Session Manager\Environment" -Name PATH -Value $newPath
#export CC=msys32/usr/bin/gcc.exe
#export CXX=msys32/usr/bin/g++.exe
