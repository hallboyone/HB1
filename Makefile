
CXX := g++
CFLAGS := -fPIC -Wall -O3 -g -Werror -Wsign-compare -Wwrite-strings -Wtype-limits -std=c++11
INC    := -I $(HDDIR)

MAJOR := 0
MINOR := 1
VERSION := $(MAJOR).$(MINOR)

NAMESPACE:= HB1
SRCDIR   := src
BUILDDIR := build
HDDIR    := include
LIBDIR   := lib/HB1

#List of all the src file names (src/mylib1/file1.c src/mylib2/file2.c src/mylib2/file3.c)
SOURCES  := $(shell find $(SRCDIR) -type f -name *.cpp)

#Removes the leading src/ from sources (mylib1/file1 mylib2/file2 mylib2/file3)
FULLNAMES := $(patsubst $(SRCDIR)/%.cpp,%, $(SOURCES))

#List the Library names (mylib1, mylib2, mylib2)
LIBNAMES  := $(patsubst %/,%,$(dir $(FULLNAMES)))

#List of src files (file1.c file2.c file3.c)
BASENAMES := $(notdir $(FULLNAMES))

#List of object files to make (build/mylib1/file1.o, build/mylib2/file2.o, build/mylib2/file3.o) 
OBJECTS  := $(patsubst %,$(BUILDDIR)/%,$(join $(LIBNAMES),$(patsubst %, /%.o, $(BASENAMES))))

TARGETS   :=  $(patsubst %,$(LIBDIR)%,$(patsubst %,/lib%.so, $(LIBNAMES)))

LIBSVER  := $(patsubst %, $(LIBDIR)/lib%.a.$(VERSION), $(NAMES))
LIBS     := $(patsubst %, $(LIBDIR)/lib%.so, $(NAMES))


all: $(addprefix lib,$(LIBNAMES))

#Make the shared libraries
lib%: $(BUILDDIR)/%/*.o
	@echo ld -shared -soname $(LIBDIR)/$@.so.$(MAJOR) -o $(LIBDIR)/$@.so.$(VERSION) -lc $^


$(LIBDIR)/libHB1.a: $(OBJECTS)
	@echo "ar rcs $@ $<"; ar rcs $@ $<


$(BUILDDIR)/%.o: $(SRCDIR)/$(%D)*
	@echo $<

libs: $(LIBSVER)


$(LIBDIR)/%.so.$(VERSION):$(BUILDDIR)/%.o
	@mkdir -p $(LIBDIR)
	ar rcs $@ $<

$(BUILDDIR)/lib%.o:$(SRCDIR)/%.cpp
	s
	@echo "$(CXX) $(CFLAGS) $(INC) -c -o $@ $<"; $(CXX) $(CFLAGS) $(INC) -c -o $@ $<

print:
	@echo "Sources       - $(SOURCES)"
	@echo "Full Names    - $(FULLNAMES)"
	@echo "Library Names - $(LIBNAMES)"
	@echo "Basenames     - $(BASENAMES)"
	@echo "Objects       - $(OBJECTS)"
	@echo "Targets       - $(TARGETS)"
	@echo $(NAMES);

	@echo $(LIBSVER)
	@echo $(LIBS)

.PHONEY: libs print
