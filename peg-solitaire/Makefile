CXX = g++
CXXFLAGS = -std=c++20 -O2
INC = -I./include

all: peg_solitaire

peg_solitaire: ./src/main.cpp ./src/peg-solitaire.cpp
	$(CXX) $(CXXFLAGS) $(INC) -o peg_solitaire $^

clean:
	rm -f *.o peg_solitaire
