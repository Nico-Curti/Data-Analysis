#required OPENMP 4.0 (since gcc 4.9)
HPP = ./lib
OUT = ./bin
TST = ./tst
STD = -std=c++11
fmath = -ffast-math
#fmath = -ffp-mode=fast
OMP  = -fopenmp

ifeq ifeq ($(shell uname -o),Cygwin)
    gnu = -std=gnu++11
endif

all: 	dir_tree \
		fstat \
		cross_val \
		neural_net \
		pca \
		bayes_cls \
		kabsch
		#fbp \
		#rsgd \

#fbp:		$(TST)/runtest_bcrfbp.cpp \
#			$(HPP)/FocusingBeliefPropagation.hpp \
#			$(HPP)/os.hpp \
#			$(HPP)/parse_args.hpp \
#			$(HPP)/fmath.hpp \
#			$(HPP)/patterns.hpp \
#			$(HPP)/score_coef.hpp 
#
#		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_fbp 		$(TST)/runtest_bcrfbp.cpp
#
#rsgd:		$(TST)/runtest_bcrsgd.cpp \
#			$(HPP)/ReplicatedSGD.hpp \
#			$(HPP)/os.hpp \
#			$(HPP)/parse_args.hpp \
#			$(HPP)/fmath.hpp \
#			$(HPP)/patterns.hpp \
#			$(HPP)/score_coef.hpp 
#
#		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_rsgd 		$(TST)/runtest_bcrsgd.cpp

fstat:		$(TST)/runtest_fstat.cpp \
			$(HPP)/timing_func.hpp \
			$(HPP)/fstat.hpp 

		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_fstat		$(TST)/runtest_fstat.cpp

cross_val:	$(TST)/runtest_cv.cpp \
			$(HPP)/cross_validation.hpp 

		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_cv		$(TST)/runtest_cv.cpp

neural_net:	$(TST)/runtest_nn.cpp \
			$(HPP)/classifier.hpp \
			$(HPP)/neural_network.hpp \
			$(HPP)/fmath.hpp \
			$(HPP)/patterns.hpp 

		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_nn		$(TST)/runtest_nn.cpp

pca:		$(TST)/runtest_pca.cpp \
			$(HPP)/dimensionality_reduction.hpp \
			$(HPP)/fstat.hpp \
			$(HPP)/fmath.hpp \
			$(HPP)/patterns.hpp

		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_pca		$(TST)/runtest_pca.cpp

bayes_cls:	$(TST)/runtest_bayes_cls.cpp \
			$(HPP)/classifier.hpp \
			$(HPP)/diagQDA.hpp \
			$(HPP)/QDA.hpp

		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_bayes_cls	$(TST)/runtest_bayes_cls.cpp

kabsch:		$(TST)/runtest_kabsch.cpp \
			$(HPP)/kabsch.hpp \
			$(HPP)/fmath.hpp \
			$(HPP)/points.hpp

		$(CXX) $(STD) $(fmath) $(gnu) $(OMP)  -O3 -I $(HPP) -o $(OUT)/runtest_kabsch	$(TST)/runtest_kabsch.cpp

dir_tree:
		@mkdir -p bin

