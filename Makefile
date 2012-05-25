all: lempel_ziv.h lempel_ziv77.h lempel_ziv.cpp lempel_ziv77.cpp main.cpp
	g++ lempel_ziv.cpp lempel_ziv77.cpp main.cpp
