# For the right Permission
# Set-ExecutionPolicy Bypass -Scope Process -Force;
Write-Host "Installing Data-Analysis dependecies:"
Write-Host "- cmake"
Write-Host "- g++ (> 4.9)"
Write-Host "- Chocolatey (recomended)"
Write-Host "- Ninja"
Write-Host "- Python3 (snakemake)"

If( $args[1] -eq $null ) # specify path to install programs
{
    $path2out = "toolchain"
}
Else
{
    $path2out = $args[1]
}

Push-Location
Set-Location $env:HOMEDRIVE$env:HOMEPATH
New-Item -Path $path2out -ItemType directory -Force
Set-Location $path2out

Write-Host "Looking for packages..."
Write-Host "CMAKE identification"
If( -Not (Get-Command cmake -ErrorAction SilentlyContinue) ) # cmake not installed
{
    Write-Host CMAKE not FOUND
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Write-Host "Download CMAKE from https://cmake.org/files/v3.10/cmake-3.10.1-win64-x64.zip"
        $url = "https://cmake.org/files/v3.10/cmake-3.10.1-win64-x64.zip"
        $out_dir = $url.split('/')[-1]
        $out = $out_dir.Substring(0, $out_dir.Length - 4)
        Start-BitsTransfer -Source $url

        Write-Host unzip $out_dir
        Expand-Archive $out_dir -DestinationPath cmake
        $env:PATH = $env:PATH + ";$PWD\cmake\$out\bin\"
        -join('$env:PATH = $env:PATH', " + `";$PWD\cmake\$out\bin\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        Remove-Item $out_dir -Force -Recurse -ErrorAction SilentlyContinue
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host Abort CMAKE installation
        }
        Else
        {
            Write-Host "Download CMAKE from https://cmake.org/files/v3.10/cmake-3.10.1-win64-x64.zip"
            $url = "https://cmake.org/files/v3.10/cmake-3.10.1-win64-x64.zip"
            $out_dir = $url.split('/')[-1]
            $out = $out_dir.Substring(0, $out_dir.Length - 4)
            Start-BitsTransfer -Source $url

            Write-Host unzip $out_dir
            Expand-Archive $out_dir -DestinationPath cmake
            $env:PATH = $env:PATH + ";$PWD\cmake\$out\bin\"

            -join('$env:PATH = $env:PATH', " + `";$PWD\cmake\$out\bin\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            Remove-Item $out_dir -Force -Recurse -ErrorAction SilentlyContinue
        }
    }
}
Else
{
    Write-Host CMAKE FOUND
}


## install gcc new version for OpenMP 4. support
Write-Host "C++ compiler identification (g++ version greater than 4.9)"
$gcc = Get-Command g++ -ErrorAction SilentlyContinue | Select-Object -ExpandProperty Definition
If( $gcc -eq $null)
{
    $version = ""
}
Else
{
    $version = & g++ "--version"
}
If( $gcc -eq $null ) # g++ not found
{
    Write-Host g++ not installed
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Start-BitsTransfer -Source https://sourceforge.net/projects/msys2/files/Base/i686/msys2-base-i686-20161025.tar.xz
        cmake -E tar zxf msys2-base-i686-20161025.tar.xz
        Remove-Item msys2-base-i686-20161025.tar.xz -Force -Recurse -ErrorAction SilentlyContinue
        Set-Location msys32
        ./msys2_shell.cmd
        ./usr/bin/pacman -Syuu --noconfirm
        ./usr/bin/pacman -S --noconfirm --needed base-devel mingw-w64-i686-toolchain #mingw-w64-x86_64-toolchain
        
        $env:PATH = $env:PATH + ";$PWD\mingw32\bin\"
        -join('$env:PATH = $env:PATH', " + `";$PWD\mingw32\bin\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        $env:CC = "$PWD\mingw32\bin\gcc.exe"
        -join('$env:CC', " = $PWD\mingw32\bin\gcc.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        $env:CXX = "$PWD\mingw32\bin\g++.exe"
        -join('$env:CXX', " = $PWD\mingw32\bin\g++.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host "Abort g++ installation"
        }
        Else
        {
            Start-BitsTransfer -Source https://sourceforge.net/projects/msys2/files/Base/i686/msys2-base-i686-20161025.tar.xz
            cmake -E tar zxf msys2-base-i686-20161025.tar.xz
            Remove-Item msys2-base-i686-20161025.tar.xz -Force -Recurse -ErrorAction SilentlyContinue
            Set-Location msys32
            ./msys2_shell.cmd
            ./usr/bin/pacman -Syuu --noconfirm
            ./usr/bin/pacman -S --noconfirm --needed base-devel mingw-w64-i686-toolchain #mingw-w64-x86_64-toolchain
            
            $env:PATH = $env:PATH + ";$PWD\mingw32\bin\"
            -join('$env:PATH = $env:PATH', " + `";$PWD\mingw32\bin\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            $env:CC = "$PWD\mingw32\bin\gcc.exe"
            -join('$env:CC', " = $PWD\mingw32\bin\gcc.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            $env:CXX = "$PWD\mingw32\bin\g++.exe"
            -join('$env:CXX', " = $PWD\mingw32\bin\g++.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        }
    }
}
ElseIf( $version.split(' ')[6].split('.')[0] -lt 4 ) # version too old
{
    Write-Host g++ version too old for OpenMP 4
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Start-BitsTransfer -Source https://sourceforge.net/projects/msys2/files/Base/i686/msys2-base-i686-20161025.tar.xz
        cmake -E tar zxf msys2-base-i686-20161025.tar.xz
        Remove-Item msys2-base-i686-20161025.tar.xz -Force -Recurse -ErrorAction SilentlyContinue
        Set-Location msys32
        ./msys2_shell.cmd
        ./usr/bin/pacman -Syuu --noconfirm
        ./usr/bin/pacman -S --noconfirm --needed base-devel mingw-w64-i686-toolchain #mingw-w64-x86_64-toolchain
        $gcc = $gcc.Substring(0, $gcc.Length - 8)
        $env:PATH = $env:PATH + ";$PWD\mingw32\bin\"
        -join('$env:PATH = $env:PATH', " + `";$PWD\mingw32\bin\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        $env:PATH = $env:PATH - ";$gcc"
        -join('$env:PATH = $env:PATH', " - `";$gcc`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        $env:CC = "$PWD\mingw32\bin\gcc.exe"
        -join('$env:CC', " = $PWD\mingw32\bin\gcc.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        $env:CXX = "$PWD\mingw32\bin\g++.exe"
        -join('$env:CXX', " = $PWD\mingw32\bin\g++.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install a new version? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host "Abort g++ installation"
        }
        Else
        {
            Start-BitsTransfer -Source https://sourceforge.net/projects/msys2/files/Base/i686/msys2-base-i686-20161025.tar.xz
            cmake -E tar zxf msys2-base-i686-20161025.tar.xz
            Remove-Item msys2-base-i686-20161025.tar.xz -Force -Recurse -ErrorAction SilentlyContinue
            Set-Location msys32
            ./msys2_shell.cmd
            ./usr/bin/pacman -Syuu --noconfirm
            ./usr/bin/pacman -S --noconfirm --needed base-devel mingw-w64-i686-toolchain #mingw-w64-x86_64-toolchain
            $gcc = $gcc.Substring(0, $gcc.Length - 8)
            $env:PATH = $env:PATH + ";$PWD\mingw32\bin\"
            -join('$env:PATH = $env:PATH', " + `";$PWD\mingw32\bin\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            $env:PATH = $env:PATH - ";$gcc"
            -join('$env:PATH = $env:PATH', " - `";$gcc`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            $env:CC = "$PWD\mingw32\bin\gcc.exe"
            -join('$env:CC', " = $PWD\mingw32\bin\gcc.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            $env:CXX = "$PWD\mingw32\bin\g++.exe"
            -join('$env:CXX', " = $PWD\mingw32\bin\g++.exe") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        }
    }
}
Else
{
    Write-Host "A sufficient version of g++ already installed"
}

# Chocolatey installation (recomended)
Write-Host Chocolatey identification
$choco = Get-Command choco -ErrorAction SilentlyContinue
If( $choco -eq $null )
{
    Write-Host Chocolatey not FOUND
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Write-Host Chocolatey will be install in C:\ProgramData\chocoportable
        $InstallDir='C:\ProgramData\chocoportable'
        $env:ChocolateyInstall="$InstallDir"
        Set-ExecutionPolicy Bypass
        iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install it (recomended)? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host Abort Chocolatey installation
        }
        Else
        {
            Write-Host Chocolatey will be installed in C:\ProgramData\chocoportable
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
}
Else
{
    Write-Host Choco FOUND
}

Write-Host SublimeText3 identification
$sub = (Get-ChildItem "HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall") | Where-Object { $_.GetValue( "DisplayName" ) -like "*sublime*"} | Select Name
If( $sub -eq $null) # not installed
{
    Write-Host "SublimeText3 not installed (recomended)"
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Write-Host Download sublimetext3 from "https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64.zip"
        Start-BitsTransfer -Source "https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64.zip"
        
        Write-Host unzip Sublime%20Text%20Build%203143%20x64.zip
        Expand-Archive Sublime%20Text%20Build%203143%20x64.zip -DestinationPath "./sublimetext3"
        Remove-Item Sublime%20Text%20Build%203143%20x64.zip -Force -Recurse -ErrorAction SilentlyContinue
        
        Write-Host Add of alias sublime to powershell configuration
        'Set-Alias sublime "./sublimetext3/subl.exe"' | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install it (recomended)? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host Abort SublimeText3 installation
        }
        Else
        {
            Write-Host Download sublimetext3 from "https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64.zip"
            Start-BitsTransfer -Source "https://download.sublimetext.com/Sublime%20Text%20Build%203143%20x64.zip"
            
            Write-Host unzip Sublime%20Text%20Build%203143%20x64.zip
            Expand-Archive Sublime%20Text%20Build%203143%20x64.zip -DestinationPath "./sublimetext3"
            Remove-Item Sublime%20Text%20Build%203143%20x64.zip -Force -Recurse -ErrorAction SilentlyContinue
            
            Write-Host Add of alias sublime to powershell configuration
            'Set-Alias sublime "./sublimetext3/subl.exe"' | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        }
    }
}
Else
{
    Write-Host SublimeText3 already installed
}

Write-Host ninja identification
If( -Not (Get-Command ninja -ErrorAction SilentlyContinue) ) # cmake not installed
{
    Write-Host ninja not FOUND
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Write-Host "Download ninja from https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-win.zip"
        $url = "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-win.zip"
        $out_dir = $url.split('/')[-1]
        Invoke-WebRequest $url -OutFile $out_dir

        Write-Host unzip $out_dir
        Expand-Archive $out_dir -DestinationPath ninja
        $env:PATH = $env:PATH + ";$PWD\ninja\"
        -join('$env:PATH = $env:PATH', " + `";$PWD\ninja\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
        Remove-Item $out_dir -Force -Recurse -ErrorAction SilentlyContinue
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install it? [y/n]"
        If($CONFIRM -eq "N" -Or $CONFIRM -eq "n")
        {
            Write-Host Abort ninja installation
        }
        Else
        {
            Write-Host "Download ninja from https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-win.zip"
            $url = "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-win.zip"
            $out_dir = $url.split('/')[-1]
            Invoke-WebRequest $url -OutFile $out_dir

            Write-Host unzip $out_dir
            Expand-Archive $out_dir -DestinationPath ninja
            $env:PATH = $env:PATH + ";$PWD\ninja\"
            -join('$env:PATH = $env:PATH', " + `";$PWD\ninja\`"") | Out-File -FilePath "$env:UserProfile\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1" -Append -Encoding ASCII
            Remove-Item $out_dir -Force -Recurse -ErrorAction SilentlyContinue
        }
    }
}
Else
{
    Write-Host ninja FOUND
}

#Set-Variable -Name "OMP_CANCELLATION" -Value True -Option constant -Scope global -Description "Set cancellation of the parallel loops"

# Miniconda3 Installer
Write-Host Python3 identification
$pyver = python -c "import sys; print(sys.version)" # python version
If($pyver -like "*3.*" -Or $pyver -like "*Anaconda 3.*") # right version 3. so install snakemake
{
    Write-Host Python3 FOUND
    Write-Host snakemake identification
    If( -Not (Get-Command snakemake -ErrorAction SilentlyContinue) )
    {
        Write-Host snakemake not FOUND
        If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
        {
            conda update conda -y
            conda config --add channels bioconda
            pip install seaborn pandas ipython numpy matplotlib snakemake graphviz
        }
        Else
        {
            $CONFIRM = Read-Host -Prompt "Do you want install snakemake and other dependecies? [y/n]"
            If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n')
            {
                Write-Host Abort
            }
            Else
            {
                conda update conda -y
                conda config --add channels bioconda
                pip install seaborn pandas ipython numpy matplotlib snakemake graphviz
            }
        }
    }
    Else
    {
        Write-Host snakemake FOUND
    }
}
ElseIf($pyver -like "*2.*" -Or $pyver -like "*Anaconda 2.*")
{
    Write-Host The Python version found is too old for snakemake
    Write-Host Please set a new version of Python as default
    Write-Host OR
    Write-Host Uninstall the current version and re-run this script for a Python3 installation
}
Else
{
    Write-Host Python not FOUND
    If( $args[0] -eq "-y" -Or $args[0] -eq "-Y" -Or $args[0] -eq "yes" )
    {
        Write-Host Download Miniconda3 from https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe
        Start-BitsTransfer -Source "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe"

        Write-Host Miniconda3 will be install and it will set as default python
        start /wait "" Miniconda3-latest-Windows-x86_64.exe /InstallationType=JustMe /RegisterPython=1 /S /D=%UserProfile%\Miniconda3
        conda update conda -y
        conda config --add channels bioconda
        pip install seaborn pandas ipython numpy matplotlib snakemake scikit-learn graphviz
    }
    Else
    {
        $CONFIRM = Read-Host -Prompt "Do you want install Python3 and other dependecies? [y/n]"
        If($CONFIRM -eq 'N' -Or $CONFIRM -eq 'n')
        {
            Write-Host Abort Python3 installation
            exit
        }
        Write-Host Download Miniconda3 from https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe
        Start-BitsTransfer -Source "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe"

        Write-Host Miniconda3 will be install and it will set as default python
        start /wait "" Miniconda3-latest-Windows-x86_64.exe /InstallationType=JustMe /RegisterPython=1 /S /D=%UserProfile%\Miniconda3
        conda update conda -y
        conda config --add channels bioconda
        pip install seaborn pandas ipython numpy matplotlib snakemake scikit-learn graphviz
    }
}

Pop-Location

# Install Data-Analysis Lib
Write-Host "Build Data-Analysis libraries with cmake-ninja"
Remove-Item build -Force -Recurse -ErrorAction SilentlyContinue
New-Item -Path .\build -ItemType directory -Force
Set-Location build

# cmake -T"LLVM-vs2015" -G "Visual Studio 15" "-DCMAKE_BUILD_TYPE=Release" ..
cmake -G "Ninja" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build . #--config Release
Set-Location ..
