CXX=clang++
CXXFLAGS=-pedantic -Wno-c11-extensions -std=c++20 -O3 -g -I "/opt/homebrew/Cellar/opencv/4.6.0/include/opencv4/"
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  grayscale load-display

grayscale: gray-scale.o
		$(CXX) $(LDFLAGS) -o $@ $^

load-display: load-display.o
		$(CXX) $(LDFLAGS) -o $@ $^


%.o: %.cc %.hh
		$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
		rm -rf *.o grayscale