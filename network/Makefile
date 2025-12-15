# ISP_Pipeline/network/Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall

TARGET = frame_receiver
SRC = frame_receiver.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean