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

# trying to have variables only in the rules :/
VSCRIPTPATH   = $(DEPENDDIR)/$(VFILE)$(HDRFILE)$(ERBFILE)
VSCRPTREQPATH = $(DEPENDDIR)/version_script.rb
VJSONPATH     = version.json
VFILEPATH     = $(INCLUDEDIR)/$(VFILE)$(HDRFILE)
VDEPENDPATH   = $(BUILDDIR)/$(VDEPEND)$(OUTFILE)

CC   = g++
CSTD = c++11

# made with GiNaC
LFLAGS   = -lcln -lginac
CXXFLAGS = -Wall -Werror -Wpedantic

SRCFILE  = .cpp
OUTFILE  = .o
HDRFILE  = .hpp
ERBFILE  = .erb

.PHONY: makefile all clean debug release cleandebug cleanrelease

# default rule for now is debug
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

# make the final executable
$(OUTPATH): $(OBJS)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LFLAGS) -std=$(CSTD)

# make each of the .o files if the coresponding src file is out of date...
# and require the version script recipe first
$(BUILDDIR)/%$(OUTFILE): $(SRCDIR)/%$(SRCFILE) $(HEADER) | inc_version
	$(CC) $(CXXFLAGS) -c -o $@ $< -I$(INCLUDEDIR) -std=$(CSTD)

# run the version script if any cpp/hpp file has changed, or the script itself, or version.json
inc_version: $(VFILEPATH)
$(VFILEPATH): $(SRCPATH) $(HEADER) $(VSCRIPTPATH) $(VSCRPTREQPATH) $(VJSONPATH)
	erb -T '-' $(VSCRIPTPATH) > $(VFILEPATH)

# incriment the build number for every clean build
clean:
	rm -f $(OBJS) $(OUT)
	erb -T '-' $(VSCRIPTPATH) > $(VFILEPATH)
