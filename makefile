LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system
# All .cc files are assumed to be source files
SRCS=$(wildcard *.cc)
# Dependency files, automatically generated and put into `build` folder
DEPS=$(patsubst %.cc,build/%.d,$(SRCS))
# Object files (also in `build` folder)
OBJS=$(DEPS:.d=.o)
# C compiler and its compilation flags (can be overriden on the command line)
CXX?=g++
CXXFLAGS?=-std=c++14 -O3 -Wall -g -Werror

# Default target (it comes first), conventionally called `all`
# Replace `my_app` with the name of your executable (here and below)
all:	pacorona

# Removes the build folder, but keeps generated executable if present
clean:
	rm -rf build

# Removes both the build folder and generated executable
dist-clean:	clean
	rm -f pacorona

# `my_app` is assumed to depend on all objects (i.e. on all .cc files)
pacorona:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDLIBS)

# Include generated dependencies, rebuilding them if necessary
include $(DEPS)

# Rule to build dependency files
build/%.d:	%.cc
	mkdir -p build
	$(CXX) $(CXXFLAGS) $< -MM -MT $(patsubst %.cc,build/%.o,$<) -MF $@

# Rule to build object files
build/%.o:	%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Phony rules are those that do not correspond to an actual file
.PHONY: all clean dist-clean

