CPPFLAGS = -I$(FNCS2_INSTALL)/include
LDFLAGS = -L$(FNCS2_INSTALL)/lib
LIBS = -lfncs -lczmq -lzmq

all: simA simB

simA: simA.cpp
	$(CXX) simA.cpp -o simA $(CPPFLAGS) $(LDFLAGS) $(LIBS)

simB: simB.cpp
	$(CXX) simB.cpp -o simB $(CPPFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm -f simA simB *.out *.log *.txt
