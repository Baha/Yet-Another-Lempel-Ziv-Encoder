all: lempel_ziv.h lempel_ziv78.h lempel_zivW.h lempel_ziv.cpp lempel_ziv78.cpp lempel_zivW.cpp code_pair.h binarizer.h binarizer.cpp main.cpp
	g++ lempel_ziv.cpp lempel_ziv78.cpp lempel_zivW.cpp binarizer.cpp main.cpp
