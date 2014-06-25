O := out
TOP := $(shell echo $${PWD-`pwd`})
#CXXFLAGS := -fPIC -g -Wall -std=c++0x -I$(TOP)/include
CXXFLAGS := -fPIC -g -Wall -O3 -std=c++0x -I$(TOP)/include

SRCFILES := src/dataview.cpp src/type_helper.cpp src/component.cpp src/kernel.cpp
OBJFILES := $(patsubst src/%.cpp, $(O)/%.o, $(SRCFILES))

UNAME_S := $(shell uname -s)
TARGETS :=
ifeq ($(UNAME_S),Linux)
	TARGETS := $(O)/libmicroscopes.so
endif
ifeq ($(UNAME_S),Darwin)
	TARGETS := $(O)/libmicroscopes.dylib
endif

all: $(TARGETS)

.PHONY: clean
clean: 
	rm -rf out/ microscopes/hodgepodge.{cpp,so}

$(O)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(O)/libmicroscopes.so: $(OBJFILES)
	gcc -shared -o $(O)/libmicroscopes.so $(OBJFILES)

$(O)/libmicroscopes.dylib: $(OBJFILES)
	g++ -dynamiclib -o $(O)/libmicroscopes.dylib $(OBJFILES)
