O := out
TOP := $(shell echo $${PWD-`pwd`})
CXXFLAGS := -fPIC -g -Wall -std=c++0x -I$(TOP)/include

SRCFILES := src/dataview.cpp src/type_helper.cpp
OBJFILES := $(patsubst src/%.cpp, $(O)/%.o, $(SRCFILES))

all: $(O)/libmicroscopes.so

$(O)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(O)/libmicroscopes.so: $(OBJFILES)
	gcc -shared -o $(O)/libmicroscopes.so $(OBJFILES)
