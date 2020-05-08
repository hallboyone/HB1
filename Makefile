CC := g++ # This is the main compiler                                                                                                                                               
SRCDIR := src/HB1
HDDIR := include
BUILDDIR := lib
TARGET := lib

SRCEXT := cpp
HDEXT := hpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.so))
CFLAGS  := -Wall -fPIC -Werror -Wsign-compare -Wwrite-strings -Wtype-limits -std=c++11 -O3 -shared
INC     := -I include

$(TARGET): $(OBJECTS)
	@echo " Done!"

$(BUILDDIR)/%.so: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -shared -o $(BUILDDIR)/lib$(notdir $@) $<"; $(CC) $(CFLAGS) $(INC) -o $(BUILDDIR)/lib$(notdir $@) $< 

#clean:
#	@echo " $ $(SRCDIR)/%.$(SRCEXT) Cleaning..."; 
#	@echo " $(RM) -r $(BUILDDIR)/HB1/* $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
#
#.PHONY: clean
