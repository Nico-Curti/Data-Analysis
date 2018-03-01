#!/bin/bash

# $1 = -y
# $2 = installation path from root

source ~/.bashrc

project="Data-Analysis"
log="install_$project.log"

red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`
reset=`tput sgr0`

printf ${yellow}"Installing $project dependecies:\n"${reset}
printf "  - (Conda)Python3 (snakemake)\n"
source ./shell_utils/bash/install_python.sh
printf "  - g++ (> 4.9)\n"
source ./shell_utils/bash/install_g++.sh
printf "  - make\n"
source ./shell_utils/bash/install_make.sh
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

install_python true $1 seaborn pandas ipython numpy matplotlib snakemake graphviz spyder sklearn >> $log
install_g++ true $1 >> $log
install_make true $1 >> $log
install_cmake true $1 >> $log
install_7zip true $1 >> $log
install_ninja true $1 >> $log

popd > /dev/null

echo ${yellow}"Build $project"${reset}
sh ./build.sh

