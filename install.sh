#!/bin/bash
source ~/.bashrc

red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
reset=`tput sgr0`
cmake_version="3.10.1"
ninja_version="1.8.2"

echo ${yellow}"Installing Data-Analysis dependecies:"
echo "- cmake"
source ./shell_utils/bash/install_cmake.sh
echo "- 7zip"
source ./shell_utils/bash/install_7zip.sh
echo "- Ninja"
source ./shell_utils/bash/install_ninja.sh
echo "- g++ (> 4.9)"
source ./shell_utils/bash/install_g++.sh
echo "- make"
source ./shell_utils/bash/install_make.sh
echo "- Python3 (snakemake)"
source ./shell_utils/bash/install_python.sh


if [ "$2" == "" ]; then	path2out="toolchain"; else path2out=$2; fi
echo ${yellow}"Installation path : "~/$path2out

pushd $HOME
mkdir -p $path2out
cd $path2out

echo "Looking for packages..."
# mkdir -p var/lib/dpkg/updates
# touch var/lib/dpkg/status
# mkdir ./var/lib/dpkg/info
# touch ./var/lib/dpkg/info/format
# echo 1 >> ./var/lib/dpkg/info/format
# dpkg --force-not-root --root=$HOME/tmp_gcc --log ./log.dpkg --install make_4.1-6_amd64.deb

# CMAKE Installer
echo "CMAKE identification"
if [ ! -x "cmake" ]; then
	echo ${red}CMAKE not FOUND
	up_version=${cmake_version::-2}
	if [ "$3" == "-y" ] || [ "$3" == "-Y" ] || [ "$3" == "yes" ]; then install_cmake "https://cmake.org/files/v$up_version/cmake-$cmake_version-Linux-x86_64.tar.gz" "." true;
	else
		read -p "Do you want install it? [y/n] " confirm
		if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then echo ${red}"Abort";
		else install_cmake "https://cmake.org/files/v$up_version/cmake-$cmake_version-Linux-x86_64.tar.gz" "." true;
		fi
	fi
else echo ${green}"CMAKE FOUND";
fi

# 7zip Installer
if [ ! -x "unzip" ]; then
	echo "7zip identification"
	if [ ! -x "7za" ]; then
		echo ${red}7zip not FOUND
		if [ "$3" == "-y" ] || [ "$3" == "-Y" ] || [ "$3" == "yes" ]; then install_7zip;
		else
			read -p "Do you want install it? [y/n] " confirm
			if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then echo ${red}"Abort";
			else install_7zip;
			fi
		fi
	else echo ${green}"7zip FOUND";
	fi
fi

# Ninja Installer
echo "Ninja-build identification"
if [ ! -x "ninja" ]; then
	echo ${red}Ninja not FOUND
	if [ "$3" == "-y" ] || [ "$3" == "-Y" ] || [ "$3" == "yes" ]; then install_ninja "https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-linux.zip" "." true;
	else
		read -p "Do you want install it? [y/n] " confirm
		if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then echo ${red}"Abort";
		else install_ninja "https://github.com/ninja-build/ninja/releases/download/v$ninja_version/ninja-linux.zip" "." true;
		fi
	fi
else echo ${green}"CMAKE FOUND";
fi

# Python3 Installer
echo "Python3 and snakemake identification"
if [ "$(python -c 'import sys;print(sys.version_info[0])')" -eq "3" ]; then
	echo ${green}"Python FOUND"
	echo "snakemake identification"
	if [ ! -x "snakemake" ]
		echo ${red}"snakemake not FOUND"
		if [ "$3" == "-y" ] || [ "$3" == "-Y" ] || [ "$3" == "yes" ]; then
			conda update conda -y
			conda config --add channels bioconda
            pip install seaborn pandas ipython numpy matplotlib snakemake graphviz spyder
        else
        	read -p "Do you want install it? [y/n] " confirm
        	if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then echo ${red}"Abort";
        	else
				conda update conda -y
				conda config --add channels bioconda
	            pip install seaborn pandas ipython numpy matplotlib snakemake graphviz spyder
	        fi
	    fi
	else echo ${green}"snakemake FOUND"
	fi
elif [ "$(python -c 'import sys;print(sys.version_info[0])')" -eq "2" ]; then
	echo ${red}"The Python version found is too old for snakemake"
	echo ${red}"Please set a new version of Python as default"
	echo ${red}"OR"
	echo ${red}"Uninstall the current version and re-run this script for a Python3 installation"
else
	echo ${red}"Python not FOUND"
	if [ "$3" == "-y" ] || [ "$3" == "-Y" ] || [ "$3" == "yes" ]; then install_python "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe" true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder;
	else
		read -p "Do you want install it? [y/n] " confirm
		if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then echo ${red}"Abort";
		else install_python "https://repo.continuum.io/miniconda/Miniconda3-latest-Windows-x86_64.exe" true seaborn pandas ipython numpy matplotlib snakemake graphviz spyder;
		fi
	fi
else echo ${green}"Python3 FOUND";
fi
