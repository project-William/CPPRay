CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS = -Wl,--as-needed
LDLIBS = -lSDL2 -lm

OBJECTS = main.o display.o engine.o mesh.o scene.o

BINARY = CPPRay

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS += -Og -g
else
	CXXFLAGS += -flto -O2
	LDFLAGS += -flto -O2
endif

all: $(BINARY)

CPPRay: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

main.o: main.cpp config.h display.h engine.h input.h
display.o: display.cpp display.h
engine.o: engine.cpp engine.h pathtrace.h
mesh.o: mesh.cpp mesh.h
scene.o: scene.cpp scene.h

tags:
	ctags -R .

TAGS:
	etags *.cpp *.h

clean:
	$(RM) $(OBJECTS) $(BINARY) tags TAGS
