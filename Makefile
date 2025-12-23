# Mac, g++-14(GCC14) och sdl3 genom homebrew
# Windows, g++(GCC) och sdl2 genom MSYS2 mingw-w64

# DIN KÄLLKOD-var dina egna .cpp-filer finns
SRC_DIR = src
# FILNAMNET för ditt program som skall byggas, och VAR
OBJ_NAME = play
BUILD_DIR = build/debug

# KOMPILATOR, g++-14/g++/g++-15 beroende på installation
# (problem efter uppdatering till GCC-15 på Mac, använd 'g++-14' (GNU GCC-14), eller 'g++'(Apple Clang))
# Mac GCC COMPILER (rek. GNU 'g++-14')!
#CC = g++-14
#CC = g++-15
#CC = g++

# Windows GCC COMPILER
CC = g++

# Valbara kompileringsflaggor(options)
COMPILER_FLAGS = -std=c++23 -Wall -O0 -g

# ALLA filer med filändelsen .cpp i foldern SRC_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# INKLUDERINGSFILER–var dina header-filer finns
# Mac INTEL INCLUDE_PATHS!
#INCLUDE_PATHS = -Iinclude -I/usr/local/include
# Mac ARM INCLUDE_PATHS!
#INCLUDE_PATHS = -Iinclude -I/opt/homebrew/include
# Windows INCLUDE_PATHS!
INCLUDE_PATHS = -Iinclude -IC:/msys64/ucrt64/include

# BIBLIOTEKSFILER–kompilerad objektkod
# Mac ARM LIBRARY_PATHS!
#LIBRARY_PATHS = -Llib -L/opt/homebrew/lib
# Mac INTEL LIBRARY_PATHS!
#LIBRARY_PATHS = -Llib -L/usr/local/lib
# Windows LIBRARY_PATHS
LIBRARY_PATHS = -Llib -LC:/msys64/ucrt64/lib

# LÄNKNING - objekfiler som används vid länkning. Enklare program utan SDL behöver normalt inte några speciella länk-flaggor
# Default LINKER_FLAGS 
#LINKER_FLAGS =

# Windows default LINKER_FLAGS to enable std::print (C++23)
LINKER_FLAGS = -lstdc++exp

# Mac LINKER_FLAGS, Om SDL3 används! 
#LINKER_FLAGS = -lSDL3 -lSDL3_image -lSDL3_ttf
# Mac LINKER_FLAGS, Om SDL2 används! 
#LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Windows LINKER_FLAGS, Om SDL3 används! 
#LINKER_FLAGS = -lmingw32 -lstdc++exp -lSDL3 -lSDL3_image -lSDL3_ttf
# Windows LINKER_FLAGS, Om SDL2 används! 
#LINKER_FLAGS = -lmingw32 -lstdc++exp -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf


all:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRC_FILES) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)
