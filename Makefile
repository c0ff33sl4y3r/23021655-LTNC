#CC specifies which compiler we're using
CC = g++

CXXFLAGS = -std=c++17
#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I src/include -I src/class

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L /opt/homebrew/lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS =

#LINKER_FLAGS specifies the libraries we're linking against
# Correct configuration for Windows and Linux
# Windows configuration
# LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
# Linux configuration
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our executable
OBJ_NAME = main

#OBJ_TARGET specifies the name of our object files
OBJ_TARGET = main.cpp

#This is the target that compiles our executable
all :
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) -o $(OBJ_NAME) $(OBJ_TARGET) $(LINKER_FLAGS) $(CXXFLAGS) && ./main

run :
	./main

# Optional: clean target to remove compiled files
clean:
	rm -f $(OBJ_NAME)