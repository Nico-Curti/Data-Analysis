#!/bin/bash

echo "Installing Data-Analysis dependecies:"
echo "- g++ (> 4.9)"
echo "- make"
echo "- 7zip"
echo "- cmake"
echo "- Ninja"
echo "- Python3 (snakemake)"

if [ "$2" == "" ]; then
	path2out="toolchain"
else
	path2out=$2
fi

pushd $HOME
mkdir -p $path2out
cd $path2out

echo "Looking for packages..."
echo "C++ compiler identification (g++ version greater than 4.9)"

## install make
#if [ ! -x make ]; then
#	echo "installing temporary version of make"
#	install_make=1
#	apt-get download make
#	mkdir tmp_make
#	dpkg -x make*.deb tmp_make
#	rm -rf make*.deb
#	export PATH=$PATH:$PWD/tmp_make/usr/bin
#	#echo export PATH='$PATH':$PWD/tmp_make/usr/bin >> ~/.bashrc
#fi

## install g++
#if [ ! -x g++ ]; then
#	echo "installing temporary version of g++"
#	#wget "http://releases.llvm.org/5.0.1/clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz"
#	#mkdir tmp_compiler
#	#mv clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz tmp_compiler/
#	#cd tmp_compiler
#	#tar xvf clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
#	#rm clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
#	#export CXX=$PWD/clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04/bin/clang++
#	#export CC=$PWD/clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04/bin/clang
#	#export PATH=$PATH:$PWD/clang+llvm-5.0.1-x86_64-linux-gnu-ubuntu-16.04/bin/
#	install_gcc=1
#	apt-get download cpp 
#	versione=$(find . -name "*.deb" | cut -d'a' -f 2 | cut -d'.' -f 1)
#	apt-get download binutils cpp-$versione g++-$versione gcc gcc-$versione gcc-$versione-base libasan2 libatomic1 libc-dev-bin libc6 libc6-dev libcc1-0 libcilkrts5 libgcc-$versione-dev libgomp1 libisl15 libitm1 liblsan0 libmpc3 libmpx0 libquadmath0 libstdc++-$versione-dev libstdc++6 libtsan0 libubsan0 linux-libc-dev manpages-dev
#	mkdir tmp_gcc
#	for file in $(find . -name "*.deb"); do 
#		dpkg -x $file tmp_gcc
#		rm -rf $file
#	done
#	export LD_LIBRARY_PATH=$PWD/tmp_gcc/usr/lib/x86_64-linux-gnu/:$PWD/tmp_gcc/usr/lib/gcc/x86_64-linux-gnu/$versione/:$LD_LIBRARY_PATH
#	export LIBRARY_PATH=$PWD/tmp_gcc/usr/lib/x86_64-linux-gnu/:$PWD/tmp_gcc/usr/lib/gcc/x86_64-linux-gnu/$versione/:$LIBRARY_PATH
#	export COMPILER_PATH=$PWD/tmp_gcc/usr/lib/x86_64-linux-gnu/:$PWD/tmp_gcc/usr/lib/gcc/x86_64-linux-gnu/$versione/:$COMPILER_PATH
#	#export GCC_EXEC_PREFIX=$PWD/tmp_gcc/
#	export C_INCLUDE_PATH=$PWD/tmp_gcc/usr/include/:$C_INCLUDE_PATH
#	export C_INCLUDE_PATH=$PWD/tmp_gcc/usr/include/x86_64-linux-gnu/:$C_INCLUDE_PATH
#	export CPLUS_INCLUDE_PATH=$PWD/tmp_gcc/usr/include/c++/$versione/:$C_INCLUDE_PATH:$CPLUS_INCLUDE_PATH
#	export PATH=$PATH:$PWD/tmp_gcc/usr/bin/
#	export CC=$PWD/tmp_gcc/usr/bin/gcc-5
#	export CXX=$PWD/tmp_gcc/usr/bin/g++-5
#	#echo export PATH='$PATH':$PWD/tmp_gcc/usr/bin >> ~/.bashrc
#fi

# install gcc new version for OpenMP 4.0 support
if [ -x g++ ]; then GCCVER=$(gcc --version | awk '/gcc /{print $0;exit 0;}' | cut -d' ' -f 4 | cut -d'.' -f 1 ); fi
#if [ "$GCCVER" == "" ]; then
#	echo "Something went very wrong!!!"
#	exit
if [[ "$GCCVER" -lt "5" || install_gcc ]]; then
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
			echo export PATH='$PATH':$PWD/$OUT_DIR/bin >> ~/.bashrc
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
				echo export PATH='$PATH':$PWD/$OUT_DIR/bin >> ~/.bashrc
			fi
		fi
	else
		echo $1 "FOUND"
	fi
}

#export OMP_CANCELLATION=true
install "cmake" "https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.tar.gz" "$1"
install "ninja" "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-linux.zip" "$1"

# install python
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
		rm ./Miniconda3-latest-Linux-x86_64.sh
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
			rm ./Miniconda3-latest-Linux-x86_64.sh
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
