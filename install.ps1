# For the right Permission
# Set-ExecutionPolicy Bypass -Scope Process -Force;

# Chocolatey installation (recomended)
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
    pip install seaborn pandas ipython numpy matplotlib snakemake
}

