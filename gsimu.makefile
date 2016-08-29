CC = g++
CXXFLAGS = -O2

target = pedsort extped pgsnp pdrop cmerge ggtpt

all:	$(target)
.PHONY:	clean
clean:
	rm -f $(target)
