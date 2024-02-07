CXX = g++
CXXFLAGS = *.c
TARGET = myprogram

build: 
	$(CXX) $(CXXFLAGS) -o $(TARGET)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

clean:
	rm  $(TARGET)



