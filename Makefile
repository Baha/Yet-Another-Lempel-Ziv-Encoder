all: lempel_ziv.h lempel_ziv78.h lempel_ziv.cpp lempel_ziv78.cpp code_pair.h code_pair.cpp main.cpp
	g++ lempel_ziv.cpp lempel_ziv78.cpp code_pair.cpp main.cpp
