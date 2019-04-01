CC = g++
CXXFLAGS = -O2 -std=c++11

target = fntpcd

all:	$(target)
fntpcd:	fntpcd.cpp chol.cpp amatrix.cpp pedigree.hpp
.PHONY:	clean
clean:
	rm -f $(target)
