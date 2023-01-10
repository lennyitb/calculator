FILENAMES = main nodecontainer nodeoperators stack runtime parser nativecmd
_HEADERS  = calc.hpp nodecontainer.hpp nodeoperators.hpp stack.hpp runtime.hpp parser.hpp nativecmd.hpp cmdlookup.hpp 

DEBUGFLAGS   = -g -DDEBUG
RELEASEFLAGS = -Ofast -DRELEASE

BUILDDIR    = build
SRCDIR      = src
INCLUDEDIR  = include
OUT         = bin
OUTPATH     = $(BUILDDIR)/$(OUT)

VFILE       = version
VDEPEND     = main
DEPENDDIR   = build_depend

VSCRIPTPATH = $(DEPENDDIR)/$(VFILE)$(HDRFILE)$(ERBFILE)
VSCRPTREQPATH = $(DEPENDDIR)/version_script.rb
VFILEPATH   = $(INCLUDEDIR)/$(VFILE)$(HDRFILE)
VDEPENDPATH = $(BUILDDIR)/$(VDEPEND)$(OUTFILE)

CC   = g++
CSTD = c++11

LFLAGS   = -lcln -lginac
CXXFLAGS = -Wall -Werror -Wpedantic

SRCFILE  = .cpp
OUTFILE  = .o
HDRFILE  = .hpp
ERBFILE  = .erb

.PHONY: makefile all clean debug release cleandebug cleanrelease

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: all
release: CXXFLAGS += $(RELEASEFLAGS)
release: all
cleandebug: clean debug
cleanrelease: clean release

all: $(OUTPATH)

OBJS    = $(patsubst %, $(BUILDDIR)/%$(OUTFILE),$(FILENAMES))
SOURCE  = $(patsubst %, %$(SRCFILE),$(FILENAMES))
SRCPATH = $(patsubst %, $(SRCDIR)/%,$(SOURCE))
HEADER  = $(patsubst %, $(INCLUDEDIR)/%,$(_HEADERS))

$(OUTPATH): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LFLAGS) -std=$(CSTD)

$(BUILDDIR)/%$(OUTFILE): $(SRCDIR)/%$(SRCFILE) $(HEADER) | inc_version
	$(CC) $(CXXFLAGS) -c -o $@ $< -I$(INCLUDEDIR) -std=$(CSTD)

inc_version: $(VFILEPATH)

$(VFILEPATH): $(SRCPATH) $(HEADER) $(VSCRIPTPATH) $(VSCRPTREQPATH)
	erb -T '-' $(VSCRIPTPATH) > $(VFILEPATH)

clean:
	rm -f $(OBJS) $(OUT)
	erb -T '-' $(VSCRIPTPATH) > $(VFILEPATH)
