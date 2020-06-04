TARGET_NAME = gol

#Directories
IDIR = ../include
ODIR = ../obj
TARGET_DIR = ../bin

#Compiler
CC = g++

#C Flags
CFLAGS = -Wall $(SDL2CFLAGS)

#define library names
LIBS = -lSDL2 -lSDL2_ttf -lm -lpthread 

#define library paths
LFLAGS = -L"/usr/lib/x86_64-linux-gnu" 

#define SDL2 Options
SDL2CFLAGS = -D_REENRRANT
SDL2INCLUDES = -I/usr/include/SDL2

_INCLUDES = $(wildcard $(IDIR)/*.h)
INCLUDES = $(SDL2INCLUDES) -I$(IDIR)

# define the C source files
SRCS =  $(wildcard *.cpp)

#define object files
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRCS))

MAIN = $(TARGET_DIR)/$(TARGET_NAME)

.SUFFIXES : .cpp .o

debug: CFLAGS += -g
debug: $(MAIN) clean

release: CFLAGS += -O2
release: $(MAIN) clean

$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(INCLUDES) $(LFLAGS) $(LIBS)

$(ODIR)/%.o:%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean: $(MAIN)
	rm $(OBJ)
