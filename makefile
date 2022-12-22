FILENAMES = main nodecontainer nodeoperators stack parser nativecmd
_HEADERS  = calc.hpp nodecontainer.hpp nodeoperators.hpp stack.hpp parser.hpp nativecmd.hpp cmdlookup.hpp

DEBUGFLAGS   = -g -DDEBUG
RELEASEFLAGS = -Ofast -DRELEASE

BUILDDIR   = build
SRCDIR     = src
INCLUDEDIR = include
OUT        = bin

CC   = g++
CSTD = c++11

LFLAGS   = -lcln -lginac
CXXFLAGS = -Wall -Wpedantic

SRCFILE  = .cpp
OUTFILE  = .o

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(OUT)
release: CXXFLAGS += $(RELEASEFLAGS)
release: $(OUT)
cleandebug: clean debug
cleanrelease: clean release

OBJS   = $(patsubst %, $(BUILDDIR)/%.o,$(FILENAMES))
SOURCE = $(patsubst %, %.cpp,$(FILENAMES))
HEADER = $(patsubst %, $(INCLUDEDIR)/%,$(_HEADERS))

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(BUILDDIR)/$@ $^ $(LFLAGS) -std=$(CSTD)

$(BUILDDIR)/%$(OUTFILE): $(SRCDIR)/%$(SRCFILE) $(HEADER)
	$(CC) $(CXXFLAGS) -c -o $@ $< -I$(INCLUDEDIR) -std=$(CSTD)

clean:
	rm -f $(OBJS) $(OUT)