
NAMESPACE:= HB1
SRCDIR   := src
BUILDDIR := build
HDDIR    := include
LIBDIR   := lib/HB1

INC    := -I $(HDDIR)
CXX := g++
CFLAGS := -fPIC -Wall -O3 -g -Werror -Wsign-compare -Wwrite-strings -Wtype-limits -std=c++11

MAJOR := 0
MINOR := 1
VERSION := $(MAJOR).$(MINOR)

#List of all the src file names (src/mylib1/file1.c src/mylib2/file2.c src/mylib2/file3.c)
SOURCES  := $(shell find $(SRCDIR) -type f -name *.cpp)

#Removes the leading src/ from sources (mylib1/file1 mylib2/file2 mylib2/file3)
FULLNAMES := $(patsubst $(SRCDIR)/%.cpp,%, $(SOURCES))

#List the Library names (mylib1, mylib2, mylib2)
LIBNAMES  := $(patsubst %/,%,$(dir $(FULLNAMES)))

#List of src files (file1.c file2.c file3.c)
BASENAMES := $(notdir $(FULLNAMES))

#List of object files to make (build/mylib1_file1.o, build/mylib2/file2.o, build/mylib2/file3.o) 
OBJECTS  := $(patsubst %,$(BUILDDIR)/%,$(join $(LIBNAMES),$(patsubst %, /%.o,$(BASENAMES))))
getobjs   = $(filter $(BUILDDIR)/$(1)/%, $(OBJECTS))

TARGETS   :=  $(patsubst %,$(LIBDIR)%,$(patsubst %,/lib%.so.$(VERSION), $(LIBNAMES)))

all: $(TARGETS)

#Make the shared libraries
.SECONDEXPANSION:
$(LIBDIR)/lib%.so.$(VERSION):$$(call getobjs,%)
	@mkdir -p $(LIBDIR)
	$(CXX) $^ -shared -Wl,-soname,$(LIBDIR)/lib$*.so.$(MAJOR) -o $@

#Make the object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)/$(dir $*)
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

print:
	@echo "Sources       - $(SOURCES)"
	@echo "Full Names    - $(FULLNAMES)"
	@echo "Library Names - $(LIBNAMES)"
	@echo "Basenames     - $(BASENAMES)"
	@echo "Objects       - $(OBJECTS)"
	@echo "Objects Lib1  - $(call getobjs,lib1)"
	@echo "Targets       - $(TARGETS)"

	@echo $(NAMES);

	@echo $(LIBSVER)
	@echo $(LIBS)

.PHONEY: libs print
