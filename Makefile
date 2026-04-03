CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
TARGET = airport
SOURCES = main.cpp airport.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

main.o: main.cpp airport.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

airport.o: airport.cpp airport.hpp
	$(CXX) $(CXXFLAGS) -c airport.cpp -o airport.o

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run