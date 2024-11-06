CXX = g++
CXXFLAGS = -std=c++20 -O2
INC = -I./include
BUILD_DIR = build

all: $(BUILD_DIR)/peg-solitaire

$(BUILD_DIR)/peg-solitaire: $(BUILD_DIR)/main.o $(BUILD_DIR)/peg-solitaire.o
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $^

$(BUILD_DIR)/main.o: ./src/main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR)/peg-solitaire.o: ./src/peg-solitaire.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
