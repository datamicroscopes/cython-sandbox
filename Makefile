O := out
TOP := $(shell echo $${PWD-`pwd`})
CXXFLAGS := -fPIC -g -MD -Wall -std=c++0x -I$(TOP)/include -I$(HOME)/distributions/include
#CXXFLAGS := -fPIC -g -MD -Wall -O3 -DNDEBUG -std=c++0x -I$(TOP)/include -I$(HOME)/distributions/include
LDFLAGS := -ldistributions_shared -L$(HOME)/distributions-bin/lib -Wl,-rpath,$(HOME)/distributions-bin/lib

SRCFILES := $(wildcard src/*.cpp src/common/*.cpp src/mixture/*.cpp src/kernels/*.cpp src/models/*.cpp) 
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

$(O)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(O)/libmicroscopes.so: $(OBJFILES)
	gcc -shared -o $(O)/libmicroscopes.so $(OBJFILES) $(LDFLAGS)

$(O)/libmicroscopes.dylib: $(OBJFILES)
	g++ -dynamiclib -o $(O)/libmicroscopes.dylib $(OBJFILES) $(LDFLAGS)

DEPFILES := $(wildcard out/*.d out/common/*.d out/mixture/*.d out/models/*.d)
ifneq ($(DEPFILES),)
-include $(DEPFILES)
endif

.PHONY: clean
clean: 
	rm -rf out/
	find microscopes/ -name '*.cpp' -o -name '*.so' -type f -print0 | xargs -0 rm --
