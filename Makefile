CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc

SRCS = src/main.cpp \
       src/linear/linear-alloc.cpp \
       src/stack/stack-alloc.cpp \
       src/pool/pool-alloc.cpp

TARGET = mini-malloc

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
