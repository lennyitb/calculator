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

CXXFLAGS =
LFLAGS   =

OBJS   = $(patsubst %, $(BUILDDIR)/%.o,$(FILENAMES))
SOURCE = $(patsubst %, %.cpp,$(FILENAMES))
HEADER = $(patsubst %, $(INCLUDEDIR)/%,$(_HEADERS))

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(OUT)
release: CXXFLAGS += $(RELEASEFLAGS)
release: $(OUT)

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) $(CXXFLAGS) -o $(BUILDDIR)/$@ $^ $(LFLAGS) -std=$(CSTD)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADER)
	$(CC) $(CXXFLAGS) -c -o $@ $< $(LFLAGS) -I$(INCLUDEDIR) -std=$(CSTD)

clean:
	rm -f $(OBJS) $(OUT)