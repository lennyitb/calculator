FILENAMES = main nodecontainer nodeoperators stack runtime parser nativecmd
_HEADERS  = calc.hpp nodecontainer.hpp nodeoperators.hpp stack.hpp runtime.hpp parser.hpp nativecmd.hpp cmdlookup.hpp 

DEBUGFLAGS   = -g -DDEBUG
RELEASEFLAGS = -Ofast -DRELEASE

BUILDDIR    = build
SRCDIR      = src
INCLUDEDIR  = include
OUT         = bin
VSCRIPTPATH = build_depend/version.hpp.erb
VFILEPATH   = include/version.hpp

CC   = g++
CSTD = c++11

LFLAGS   = -lcln -lginac
CXXFLAGS = -Wall -Werror -Wpedantic

SRCFILE  = .cpp
OUTFILE  = .o

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(OUT)
release: CXXFLAGS += $(RELEASEFLAGS)
release: $(OUT)
cleandebug: clean debug
cleanrelease: clean release

OBJS   = $(patsubst %, $(BUILDDIR)/%$(OUTFILE),$(FILENAMES))
SOURCE = $(patsubst %, %$(SRCFILE),$(FILENAMES))
HEADER = $(patsubst %, $(INCLUDEDIR)/%,$(_HEADERS))

.PHONY: inc_version clean

all: $(OUT)

$(OUT): $(OBJS) | build/main.o
	$(CC) $(CXXFLAGS) -o $(BUILDDIR)/$@ $^ $(LFLAGS) -std=$(CSTD)

build/main.o: inc_version

$(BUILDDIR)/%$(OUTFILE): $(SRCDIR)/%$(SRCFILE) $(HEADER)
	$(CC) $(CXXFLAGS) -c -o $@ $< -I$(INCLUDEDIR) -std=$(CSTD)

inc_version:
	erb build_depend/version.hpp.erb | tee include/version.hpp

clean:
	rm -f $(OBJS) $(OUT)
