CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

TARGET = build/burgundis

SRCS = src/main.cpp src/server.cpp
OBJS = $(SRCS:src/%.cpp=build/%.o)

build/%.o: src/%.cpp src/server.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -rf build

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run