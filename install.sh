
function install # $1 program name, $2 url
{
	echo $1 "installation required"
	PROG="$(which $1)"
	if [ "$PROG" == "" ]; then
		echo $1 "not installed"
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
				tar zxvf $OUT_DIR > /tmp/null
				OUT="${OUT_DIR%.*}"
				OUT="${OUT%.*}"
			elif [[ "$OUT_DIR" == *".zip" ]]; then
				echo "Unzip" $OUT_DIR
				unzip "$OUT_DIR"
				OUT="${OUT_DIR%.*}"
			else
				echo "already uncompressed"
			fi
			rm -rf "$OUT_DIR"*
			IFS="-" read -ra OUT_DIR <<< "$OUT"
			mv ${OUT} $OUT_DIR
			source ~/.bashrc
			export PATH=$PATH:$OUT_DIR/bin
		fi
	else
		echo $1 "already installed"
	fi
}

#export OMP_CANCELLATION=true
install "cmake" "https://cmake.org/files/v3.10/cmake-3.10.0-Linux-x86_64.tar.gz"
install "ninja" "https://github.com/ninja-build/ninja/releases/download/v1.8.2/ninja-win.zip"

echo "Python3 and snakemake installation required"
PROG="$(which python)"
if [ "$PROG" == "" ]; then
	echo "Python not installed"
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
		pip install -U numpy matplotlib seaborn networkx scikit-learn pandas snakemake
		rm rm ./Miniconda3-latest-Linux-x86_64.sh
elif [ "$(python -c 'import sys;print(sys.version_info[0])')" -eq "3" ]; then
	echo "Python3 already installed"
	conda update conda -y
	conda config --add channels bioconda
	pip install -U numpy matplotlib seaborn networkx scikit-learn pandas snakemake
else
	echo "Current python version is too old for snakemake"
	echo "Please uninstall your python and re-run this script."
fi
