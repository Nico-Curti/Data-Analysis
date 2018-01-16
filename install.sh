#!/bin/bash

echo "Installing Data-Analysis dependecies:"
echo "- g++ (> 4.9)"
echo "- 7zip"
echo "- cmake"
echo "- Ninja"
echo "- Python3 (snakemake)"

if [ "$2" == "" ]; then
	path2out=toolchain
else
	path2out=$2
fi

pushd pwd
cd $HOME
mkdir -p $path2out
cd $path2out

echo "Looking for packages..."
echo "C++ compiler identification (g++ version greater than 4.9)"

# install gcc new version for OpenMP 4.0 support
GCCVER=$(gcc --version | awk '/gcc /{print $0;exit 0;}' | cut -d' ' -f 4 | cut -d'.' -f 1)
if [[ "$GCCVER" -lt "5" ]]; then
	echo "gcc version too old or not installed"
	if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then
		wget "ftp://ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz"
		tar xzf gcc-7.2.0.tar.gz
		mv gcc-7.2.0 gcc-7.2.0-sources
		cd gcc-7.2.0-sources
		./contrib/download_prerequisites
		cd ..
		mkdir objdir
		cd objdir
		$PWD/../gcc-7.2.0-sources/configure --prefix=$HOME/gcc-7.2.0 --enable-languages=c,c++,fortran,go
		make
		make install
		rm -rf objdir gcc-7.2.0-sources
		export CC=$HOME/gcc-7.2.0/bin/gcc
		export CXX=$HOME/gcc-7.2.0/bin/g++
		echo "export CC=$HOME/gcc-7.2.0/bin/gcc" >> ~/.bashrc
		echo "export CXX=$HOME/gcc-7.2.0/bin/g++" >> ~/.bashrc
	else
		read -p "Do you want install it? [y/n] " CONFIRM
		if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then
			echo "Abort gcc installation"
		else
			wget "ftp://ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz"
			tar xzf gcc-7.2.0.tar.gz
			mv gcc-7.2.0 gcc-7.2.0-sources
			cd gcc-7.2.0-sources
			./contrib/download_prerequisites
			cd ..
			mkdir objdir
			cd objdir
			$PWD/../gcc-7.2.0-sources/configure --prefix=$HOME/gcc-7.2.0 --enable-languages=c,c++,fortran,go
			make
			make install
			rm -rf objdir gcc-7.2.0-sources
			export CC=$HOME/gcc-7.2.0/bin/gcc
			export CXX=$HOME/gcc-7.2.0/bin/g++
			echo "export CC=$HOME/gcc-7.2.0/bin/gcc" >> ~/.bashrc
			echo "export CXX=$HOME/gcc-7.2.0/bin/g++" >> ~/.bashrc
		fi
	fi
else
	echo "gcc already installed"
fi

if [[ "$(which 7zip)" == "" ]]; then
	wget https://downloads.sourceforge.net/project/p7zip/p7zip/16.02/p7zip_16.02_src_all.tar.bz2
	tar jxvf p7zip_16.02_src_all.tar.bz2
	cd p7zip_16.02/
	cp makefile.linux_gcc6_sanitize makefile.linux
	make -j all_test
	cd ..
	export PATH=$PATH:$PWD/p7zip_16.02/bin
	echo export PATH='$PATH':$PWD/p7zip_16.02/bin >> ~/.bashrc
	#./bin/7za
fi

function install # $1 program name, $2 url, $3 confirm
{
	echo $1 "identification"
	PROG="$(which $1)"
	if [ "$PROG" == "" ]; then
		echo $1 "not FOUND"
		if [ "$3" == "-y" ] || [ "$3" == "-Y" ] || [ "$3" == "yes" ]; then
			echo "Download $1 from $2"
			IFS='/' read -ra TOKEN <<< "$2"
			OUT_DIR=${TOKEN[${#TOKEN[@]}-1]}
			IFS='-' read -ra OUT <<< "$OUT_DIR"
			wget $2
			if [[ "$OUT_DIR" == *".tar.gz" ]]; then
				echo "Unzip" $OUT_DIR 
				tar zxf $OUT_DIR
				OUT="${OUT_DIR%.*}"
				OUT="${OUT%.*}"
			elif [[ "$OUT_DIR" == *".zip" ]]; then
				echo "Unzip" $OUT_DIR
				7za x "$OUT_DIR"
				OUT="${OUT_DIR%.*}"
			elif [[ "OUT_DIR" == *".tar.bz2" ]]; then
				echo "Unzip" $OUT_DIR
				tar jxf $OUT_DIR
				OUT="${OUT_DIR%.*}"
			else
				echo "already uncompressed"
				OUT="$OUT_DIR"
			fi
			rm -rf "$OUT_DIR"*
			IFS="-" read -ra OUT_DIR <<< "$OUT"
			mv ${OUT} $OUT_DIR
			source ~/.bashrc
			export PATH=$PATH:$PWD/$OUT_DIR/bin
			echo export PATH='$PATH':$OUT_DIR/bin >> ~/.bashrc
		else
			read -p "Do you want install it? [y/n] " CONFIRM
			if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then
				echo "Abort" $1 "installation"
			else
				echo "Download $1 from $2"
				IFS='/' read -ra TOKEN <<< "$2"
				OUT_DIR=${TOKEN[${#TOKEN[@]}-1]}
				IFS='-' read -ra OUT <<< "$OUT_DIR"
				wget $2
				if [[ "$OUT_DIR" == *".tar.gz" ]]; then
					echo "Unzip" $OUT_DIR 
					tar zxf $OUT_DIR
					OUT="${OUT_DIR%.*}"
					OUT="${OUT%.*}"
				elif [[ "$OUT_DIR" == *".zip" ]]; then
					echo "Unzip" $OUT_DIR
					7za x "$OUT_DIR"
					OUT="${OUT_DIR%.*}"
				elif [[ "OUT_DIR" == *".tar.bz2" ]]; then
					echo "Unzip" $OUT_DIR
					tar jxf $OUT_DIR
					OUT="${OUT_DIR%.*}"
				else
					echo "already uncompressed"
					OUT="$OUT_DIR"
				fi
				rm -rf "$OUT_DIR"*
				IFS="-" read -ra OUT_DIR <<< "$OUT"
				mv ${OUT} $OUT_DIR
				source ~/.bashrc
				export PATH=$PATH:$PWD/$OUT_DIR/bin
				echo export PATH='$PATH':$OUT_DIR/bin >> ~/.bashrc
			fi
		fi
	else
		echo $1 "FOUND"
	fi
}

#export OMP_CANCELLATION=true
install "cmake" "https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.tar.gz" "$1"
install "ninja" "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-linux.zip" "$1"

echo "Python3 and snakemake identification"
PROG="$(which python)"
if [ "$PROG" == "" ]; then
	echo "Python not FOUND"
	if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then
		echo "Download Miniconda3 from "https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
		wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
		chmod ugo+x Miniconda3-latest-Linux-x86_64.sh 
		bash ./Miniconda3-latest-Linux-x86_64.sh -b
		source ~/.bashrc
		export PATH=~/miniconda3/bin:$PATH 
		conda update conda -y
		conda config --add channels bioconda
		pip install -U numpy matplotlib seaborn networkx scikit-learn pandas snakemake graphviz
		rm rm ./Miniconda3-latest-Linux-x86_64.sh
	else
		read -p "Do you want install it? [y/n] " CONFIRM
		if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then
			echo "Abort Python installation"
		else
			echo "Download Miniconda3 from "https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
			wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
			chmod ugo+x Miniconda3-latest-Linux-x86_64.sh 
			bash ./Miniconda3-latest-Linux-x86_64.sh -b
			source ~/.bashrc
			export PATH=~/miniconda3/bin:$PATH 
			conda update conda -y
			conda config --add channels bioconda
			pip install -U numpy matplotlib seaborn networkx scikit-learn pandas snakemake graphviz
			rm rm ./Miniconda3-latest-Linux-x86_64.sh
		fi
	fi
elif [ "$(python -c 'import sys;print(sys.version_info[0])')" -eq "3" ]; then
	echo "Python3 FOUND"
	if [ "$1" == "-y" ] || [ "$1" == "-Y" ] || [ "$1" == "yes" ]; then
		conda update conda -y
		conda config --add channels bioconda
		pip install -U numpy matplotlib seaborn networkx scikit-learn pandas snakemake graphviz
	else
		read -p "Do you want install snakemake and other dependecies? [y/n] " CONFIRM
		if [ "$CONFIRM" == "n" ] || [ "$CONFIRM" == "N" ]; then
			echo "Abort"
		else
			conda update conda -y
			conda config --add channels bioconda
			pip install -U numpy matplotlib seaborn networkx scikit-learn pandas snakemake graphviz
		fi
	fi
else
	echo "Current python version is too old for snakemake"
	echo "Please uninstall your python and re-run this script."
fi



popd

# Install Data-Analysis Lib
echo "Build Data-Analysis libraries with cmake-ninja"

rm -rf build
mkdir -p build
cd build

cmake -G "Ninja" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build .
cd ..
