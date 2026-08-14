CXX = g++
TARGET = VMTranslator
STD = c++17
SRCS = VMTranslator.cpp CodeWriter.cpp Parser.cpp Instruction.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -std=$(STD) -o $(TARGET) $(SRCS)
	chmod 750 $(TARGET)

clean:
	rm -f $(TARGET)