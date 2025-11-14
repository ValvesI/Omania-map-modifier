CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++17
SRC = src/main.cpp src/beatmap.cpp src/manialib.cpp src/notes.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
