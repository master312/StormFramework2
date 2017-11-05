CC = g++
CFLAGS  = -g -Wall -std=c++14 -DSTORM_BUILD_PLATFORM_SDL2 -D_DEBUG -I"/usr/include/lua5.1/" -I"/usr/lib/" -I"libs/sol2/" -I"libs/" #-pg
LIBS = -w -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU -llua5.1
OBJDIR = objects
EXNAME = a.out
SOURCEDIR = src/ libs/

# Find all .cpp files
SRCS_ALL := $(shell find $(SOURCEDIR) -name '*.cpp')
# Remove all paths containing QtEditor keyword
SRCS := $(filter-out QtEditor%.cpp,$(SRCS_ALL))
# Generate all object paths... Replaces .cpp with .o
OBJECTS := $(addprefix $(OBJDIR)/,$(SRCS:%.cpp=%.o))


default: debug


$(OBJDIR)/%.o: %.cpp
	mkdir -p $(patsubst %/,%,$(dir $(basename $@))); $(CC) $(CFLAGS) -c $< $(LIBS) -o $@

debug: $(OBJECTS) $(SRCS_HPP)
	$(CC) $(CFLAGS) -o "$(EXNAME)" $(OBJECTS) main.cpp $(LIBS) 