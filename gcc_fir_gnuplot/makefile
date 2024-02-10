CXX = g++
CXXFLAGS = -std=c++11 -Wall
CXXFLAGS2 = *.c main.cpp
TARGET = myprogram



.PHONY: run clean



clean:
	rm  $(TARGET)


build: 
	$(CXX) $(CXXFLAGS) $(CXXFLAGS2) -o $(TARGET)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)


