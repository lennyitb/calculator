FILENAMES = main nodecontainer stack
_HEADERS  = calc.hpp nodecontainer.hpp stack.hpp

DEBUGFLAGS   = -g -DDEBUG
RELEASEFLAGS = -O3 -DRELEASE

BUILDDIR   = build
SRCDIR     = src
INCLUDEDIR = include
OUT        = bin

CC   = g++
CSTD = c++11

LFLAGS   = -lcln -lginac
CXXFLAGS = -Wall -Wpedantic

OBJS   = $(patsubst %, $(BUILDDIR)/%.o,$(FILENAMES))
SOURCE = $(patsubst %, %.cpp,$(FILENAMES))
HEADER = $(patsubst %, $(INCLUDEDIR)/%,$(_HEADERS))

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(OUT)
release: CXXFLAGS += $(RELEASEFLAGS)
release: $(OUT)
cleandebug: clean debug
cleanrelease: clean release

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(BUILDDIR)/$@ $^ $(LFLAGS) -std=$(CSTD)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADER)
	$(CC) $(CXXFLAGS) -c -o $@ $< -I$(INCLUDEDIR) -std=$(CSTD)

clean:
	rm -f $(OBJS) $(OUT)