#!/bin/bash

# $1 = -y
# $2 = installation path from root

source ~/.bashrc

project="Data-Analysis"

red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
reset=`tput sgr0`
cmake_version="3.10.1"
cmake_up_version="$(echo $cmake_version | cut -d'.' -f 1,2)"
ninja_version="1.8.2"

if [[ "$OSTYPE" == "darwin"* ]]; then
    url_python="https://repo.continuum.io/miniconda/Miniconda3-latest-MacOSX-x86_64.sh"
    url_cmake="https://cmake.org/files/v$cmake_up_version/cmake-$cmake_version-Darwin-x86_64.tar.gz"
    url_ninja="https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-mac.zip"
    url_gcc="ftp://ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz"
    url_7zip="https://downloads.sourceforge.net/project/p7zip/p7zip/16.02/p7zip_16.02_src_all.tar.bz2"
else
    url_python="https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh"
    url_cmake="https://cmake.org/files/v$cmake_up_version/cmake-$cmake_version-Linux-x86_64.tar.gz"
    url_ninja="https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-linux.zip"
    url_gcc="ftp://ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz"
    url_7zip="https://downloads.sourceforge.net/project/p7zip/p7zip/16.02/p7zip_16.02_src_all.tar.bz2"
fi

printf ${yellow}"Installing $project dependecies:\n"${reset}
printf "  - (Conda)Python3 (snakemake)\n"
source ./shell_utils/bash/install_python.sh
printf "  - g++ (> 4.9)\n"
printf "  - make\n"
printf "  - cmake\n"
source ./shell_utils/bash/install_cmake.sh
printf "  - ninja-build\n"
source ./shell_utils/bash/install_7zip.sh
source ./shell_utils/bash/install_ninja.sh

if [ "$2" == "" ]; then path2out="toolchain"; else path2out=$2; fi
printf ${green}"Installation path : "~/$path2out"\n"${reset}

pushd $HOME > /dev/null
mkdir -p $path2out
cd $path2out

echo "Looking for packages..."

# Python3 Installer
printf "Python3 identification: "
if [ "$(python -c 'import sys;print(sys.version_info[0])')" -eq "3" ]; then # right python version installed
    echo ${green}"FOUND"${reset}
    Conda=$(which python)
    printf "Conda identification: "
    if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
        printf "snakemake identification: "
        if [ $(which snakemake) != "" ]; then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then
                conda update conda -y > /dev/null
                conda config --add channels bioconda > /dev/null
                yes | pip install seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else
                    conda update conda -y > /dev/null
                    conda config --add channels bioconda > /dev/null
                    yes | pip install seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null
                fi
            fi
        fi # close snakemake case
        printf "g++ (> 4.9) identification: "
        if [ $(which g++) != "" ]; then 
            GCCVER=$(g++ --version | cut -d' ' -f 3);
            GCCVER=$(echo $GCCVER | cut -d' ' -f 1 | cut -d'.' -f 1); 
        fi
        if [[ "$GCCVER" -lt "5" ]]; then
            echo ${red}sufficient version NOT FOUND${reset}
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then
                conda install -y -c conda-forge isl=0.17.1 > /dev/null
                conda install -y -c creditx gcc-7 > /dev/null
                conda install -y -c gouarin libgcc-7 > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else
                    conda install -y -c conda-forge isl=0.17.1 > /dev/null
                    conda install -y -c creditx gcc-7 > /dev/null
                    conda install -y -c gouarin libgcc-7 > /dev/null
                fi
            fi
        else echo ${green}"FOUND"${reset};
        fi # close g++ case
        printf "cmake identification: "
        if [ $(which cmake) != "" ];then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then conda install -y -c anaconda cmake  > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda cmake > /dev/null;
                fi
            fi
        fi # close cmake case
        printf "ninja-build identification: "
        if [ $(which ninja) != "" ]; then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then conda install -y -c anaconda ninja > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda ninja  > /dev/null;
                fi
            fi
        fi # close ninja-build case 
        printf "make identification: "
        if [ $(which make) != "" ]; then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then conda install -y -c anaconda make > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda make > /dev/null;
                fi
            fi
        fi # close make case

    else # conda not found
        echo ${red}"NOT FOUND"${reset};
        if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then install_python $url_python true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null;
        else
            read -p "Do you want install it? [y/n] " confirm
            if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
            else install_python $url_python true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null;
            fi
        fi # close install python
        printf "g++ (> 4.9) identification: "
        if [ $(which g++) != "" ]; then 
            GCCVER=$(g++ --version | cut -d' ' -f 3);
            GCCVER=$(echo $GCCVER | cut -d' ' -f 1 | cut -d'.' -f 1); 
        fi
        if [[ "$GCCVER" -lt "5" ]] && [ ! -z "$GCCVER" ]; then # found a gcc too old
            echo ${red}version too old${reset}
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then install_g++ $url_gcc "." true > /dev/null;
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else install_g++ $url_gcc "." true > /dev/null;
                fi
            fi
        elif [ -z "$GCCVER" ]; then # g++ not FOUND
            echo ${red}NOT FOUND${reset}
            Conda=$(which python)
            printf "Conda identification: "
            if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
                if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then
                    conda install -y -c conda-forge isl=0.17.1 > /dev/null
                    conda install -y -c creditx gcc-7 > /dev/null
                    conda install -y -c gouarin libgcc-7 > /dev/null
                else
                    read -p "Do you want install it? [y/n] " confirm
                    if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                    else
                        conda install -y -c conda-forge isl=0.17.1 > /dev/null
                        conda install -y -c creditx gcc-7 > /dev/null
                        conda install -y -c gouarin libgcc-7 > /dev/null
                    fi
                fi
            else
                echo ${red}"g++ available only with conda"${reset}
                exit
            fi
        else echo ${green}"FOUND"${reset};
        fi
        printf "make identification: "
        if [ $(which make) != "" ]; then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            Conda=$(which python)
            printf "Conda identification: "
            if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
                if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda make > /dev/null
                else
                    read -p "Do you want install it? [y/n] " confirm
                    if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                    else conda install -y -c anaconda make > /dev/null;
                    fi
                fi
            else echo ${red}"make available only with conda"
            fi
        fi
        printf "cmake identification: "
        if [ $(which cmake) != "" ]; then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            Conda=$(which python)
            printf "Conda identification: "
            if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
                if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda cmake > /dev/null
                else
                    read -p "Do you want install it? [y/n] " confirm
                    if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                    else conda install -y -c anaconda cmake > /dev/null;
                    fi
                fi
            else
                if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then install_cmake $url_cmake "." true > /dev/null;
                else
                    read -p "Do you want install it? [y/n] " confirm
                    if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                    else install_cmake $url_cmake "." true > /dev/null;
                    fi
                fi
            fi
        fi
        printf "ninja-build identification: "
        if [ $(which ninja) != "" ]; then echo ${green}"FOUND"${reset};
        else
            echo ${red}"NOT FOUND"${reset}
            Conda=$(which python)
            printf "Conda identification: "
            if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
                if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda ninja > /dev/null
                else
                    read -p "Do you want install it? [y/n] " confirm
                    if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                    else conda install -y -c anaconda ninja > /dev/null;
                    fi
                fi
            else
                if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then
                    if [ $(which unzip) != "" ] || [ $(which 7za) != "" ]; then
                        install_ninja $url_ninja "." true > /dev/null;
                    elif [ $(which unzip) == "" ] || [ $(which 7za) == "" ]; then
                        install_7zip $url_7zip "." true > /dev/null;
                        install_ninja $url_ninja "." true > /dev/null;
                    fi
                else
                    read -p "Do you want install it? [y/n] " confirm
                    if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                    else 
                        if [ $(which unzip) != "" ] || [ $(which 7za) != "" ]; then
                            install_ninja $url_ninja "." true > /dev/null;
                        elif [ $(which unzip) == "" ] || [ $(which 7za) == "" ]; then
                            install_7zip $url_7zip "." true > /dev/null;
                            install_ninja $url_ninja "." true > /dev/null;
                        fi
                    fi
                fi
            fi
        fi
    fi # end conda not found
elif [ "$(python -c 'import sys;print(sys.version_info[0])')" -eq "2" ]; then   
    echo ${red}"The Python version found is too old for snakemake"${reset}
    if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then install_python $url_python true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null;
    else
        read -p "Do you want install it? [y/n] " confirm
        if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
        else install_python $url_python true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null;
        fi
    fi # close install python
    printf "g++ (> 4.9) identification: "
    if [ $(which g++) != "" ]; then 
        GCCVER=$(g++ --version | cut -d' ' -f 3);
        GCCVER=$(echo $GCCVER | cut -d' ' -f 1 | cut -d'.' -f 1); 
    fi
    if [[ "$GCCVER" -lt "5" ]] && [ ! -z "$GCCVER" ]; then # found a gcc too old
        echo ${red}g++ version too old${reset}
        if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then install_g++ $url_gcc "." true > /dev/null;
        else
            read -p "Do you want install it? [y/n] " confirm
            if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
            else install_g++ $url_gcc "." true > /dev/null;
            fi
        fi
    elif [ -z "$GCCVER" ]; then # g++ not FOUND
        echo ${red}NOT FOUND${reset}
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then
                conda install -y -c conda-forge isl=0.17.1 > /dev/null
                conda install -y -c creditx gcc-7 > /dev/null
                conda install -y -c gouarin libgcc-7 > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else
                    conda install -y -c conda-forge isl=0.17.1 > /dev/null
                    conda install -y -c creditx gcc-7 > /dev/null
                    conda install -y -c gouarin libgcc-7 > /dev/null
                fi
            fi
        else
            echo ${red}"g++ available only with conda"${reset}
            exit
        fi
    else echo ${green}"FOUND"${reset};
    fi
    printf "make identification: "
    if [ $(which make) != "" ]; then echo ${green}"FOUND"${reset};
    else
        echo ${red}"NOT FOUND"${reset}
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda make > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda make > /dev/null;
                fi
            fi
        else  echo ${red}"make available only with conda"${reset}
        fi
    fi
    printf "cmake identification: "
    if [ $(which cmake) != "" ]; then echo ${green}"FOUND"${reset};
    else
        echo ${red}"NOT FOUND"${reset}
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda cmake > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda cmake > /dev/null;
                fi
            fi
        else
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then install_cmake $url_cmake "." true > /dev/null;
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else install_cmake $url_cmake "." true > /dev/null;
                fi
            fi
        fi
    fi
    printf "ninja-build identification: "
    if [ $(which ninja) != "" ]; then echo ${green}"FOUND"${reset};
    else
        echo ${red}"NOT FOUND"${reset}
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda ninja > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda ninja > /dev/null;
                fi
            fi
        else
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then
                if [ $(which unzip) != "" ] || [ $(which 7za) != "" ]; then
                    install_ninja $url_ninja "." true > /dev/null;
                elif [ $(which unzip) == "" ] || [ $(which 7za) == "" ];then
                    install_7zip $url_7zip "." true > /dev/null;
                    install_ninja $url_ninja "." true > /dev/null;
                fi
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else 
                    if [ $(which unzip) != "" ] || [ $(which 7za) != "" ]; then
                        install_ninja $url_ninja "." true > /dev/null;
                    elif [ $(which unzip) == "" ] || [ $(which 7za) == "" ];then
                        install_7zip $url_7zip "." true > /dev/null;
                        install_ninja $url_ninja "." true > /dev/null;
                    fi
                fi
            fi
        fi
    fi
else
    echo ${red}"NOT FOUND"
    if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then install_python $url_python true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null;
    else
        read -p "Do you want install it? [y/n] " confirm
        if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
        else install_python $url_python true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn > /dev/null;
        fi
    fi # close install python
    printf "g++ (> 4.9) identification: "
    if [[ -x "/usr/bin/g++" ]]; then GCCVER=$(g++ --version | awk '/g++ /{print $0;exit 0;}' | cut -d' ' -f 4 | cut -d'.' -f 1 ); fi
        if [[ "$GCCVER" -lt "5" ]] && [ ! -z "$GCCVER" ]; then # found a gcc too old
            echo ${red}g++ version too old${reset}
        if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then install_g++ $url_gcc "." true > /dev/null;
        else
            read -p "Do you want install it? [y/n] " confirm
            if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
            else install_g++ $url_gcc "." true > /dev/null > /dev/null;
            fi
        fi
    elif [ -z "$GCCVER" ]; then # g++ not FOUND
        echo ${red}NOT FOUND${reset}
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then
                conda install -y -c conda-forge isl=0.17.1 > /dev/null
                conda install -y -c creditx gcc-7 > /dev/null
                conda install -y -c gouarin libgcc-7 > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else
                    conda install -y -c conda-forge isl=0.17.1 > /dev/null
                    conda install -y -c creditx gcc-7 > /dev/null
                    conda install -y -c gouarin libgcc-7 > /dev/null
                fi
            fi
        else
            echo ${red}"g++ available only with conda"${reset}
            exit
        fi
    else echo ${green}"FOUND";
    fi
    printf "make identification: "
    if [ $(which make) != "" ]; then echo ${green}"FOUND"${reset};
    else
        echo ${red}"NOT FOUND"${reset}
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda make > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort";
                else conda install -y -c anaconda make > /dev/null;
                fi
            fi
        else
            echo ${red}"make available only with conda"${reset}
        fi
    fi
    printf "cmake identification: "
    if [ $(which cmake) != "" ]; then echo ${green}"FOUND"${reset};
    else
        echo ${red}"NOT FOUND"
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda cmake > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda cmake > /dev/null;
                fi
            fi
        else
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then install_cmake $url_cmake "." true > /dev/null;
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else install_cmake $url_cmake "." true > /dev/null;
                fi
            fi
        fi
    fi
    printf "ninja-build identification: "
    if [ $(which ninja) != "" ]; then echo ${green}"FOUND"${reset};
    else
        echo ${red}"NOT FOUND"
        Conda=$(which python)
        printf "Conda identification: "
        if echo $Conda | grep -q "miniconda" || echo $Conda | grep -q "anaconda"; then echo ${green}"FOUND"${reset}; # CONDA INSTALLER FOUND
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then conda install -y -c anaconda ninja > /dev/null
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else conda install -y -c anaconda ninja > /dev/null;
                fi
            fi
        else
            if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ];then
                if [ $(which unzip) != "" ] || [ $(which 7za) != "" ]; then
                    install_ninja $url_ninja "." true > /dev/null;
                elif [ $(which unzip) == "" ] || [ $(which 7za) == "" ];then
                    install_7zip $url_7zip "." true > /dev/null;
                    install_ninja $url_ninja "." true > /dev/null;
                fi
            else
                read -p "Do you want install it? [y/n] " confirm
                if [ "$confirm" == "n" ] || [ "$confirm" == "N" ]; then echo ${red}"Abort"${reset};
                else 
                    if [ $(which unzip) != "" ] || [ $(which 7za) != "" ]; then
                        install_ninja $url_ninja "." true > /dev/null;
                    elif [ $(which unzip) == "" ] || [ $(which 7za) == "" ];then
                        install_7zip $url_7zip "." true > /dev/null;
                        install_ninja $url_ninja "." true > /dev/null;
                    fi
                fi
            fi
        fi
    fi

fi # close python installer


popd > /dev/null

echo ${yellow}"Build $project"${reset}
sh ./build.sh

