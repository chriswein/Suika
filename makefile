# Compiler
CC = clang++

# Compiler flags
CFLAGS = -I./include -Wall -std=c++14 -O3

# Frameworks
FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Libraries
LIBS = ./libs/libraylib.a ./libs/libbox2d.a

# Source files
SRC = main.cc box.cc helpers.cc suika.cc contactlistener.cc game.cc

# Output binary
OUTPUT = suika

all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) $(FRAMEWORKS) $^ $(LIBS) -o $@

clean:
	rm -f $(OUTPUT)

.PHONY: clean

