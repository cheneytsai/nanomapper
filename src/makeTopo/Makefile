CC=	g++
CXXFLAGS=	-g	-Wall	-mfpmath=sse	-malign-double	-mmmx	-msse	-m32	-fno-stack-protector

all:Phy

debug: PhyD
debug: CC=g++
debug: CXXFLAGS = -g

nopng: PhyN.o Topology.o OptionDB.o
	$(CC) $(CXXFLAGS) -lm -o Phy PhysicalModel.o Topology.o OptionDB.o


PhyD: PhyD.o PngD.o Topology.o OptionDB.o
	$(CC) -g -Wall -lm -lpng -o Phy PhysicalModel.o Png.o Topology.o OptionDB.o

PhyD.o: PhysicalModel.cc PhysicalModel.hh
	$(CC) -g -Wall -c PhysicalModel.cc

PngD.o: Png.cc
	$(CC) -g -Wall -c Png.cc

Phy: Phy.o Topology.o OptionDB.o Png.o Node.o
	$(CC) $(CXXFLAGS) -lm -lpng -o Phy PhysicalModel.o Topology.o OptionDB.o Png.o Node.o

OptionDB.o: OptionDB.cc OptionDB.hh
	$(CC) $(CXXFLAGS) -c OptionDB.cc

PathAnal: PathAnal.o
	$(CC) $(CXXFLAGS) -lm -o PA PathAnal.o

PathAnal.o: PathAnal.cc
	$(CC) $(CXXFLAGS) -c PathAnal.cc

Phy.o: PhysicalModel.cc PhysicalModel.hh
	$(CC) $(CXXFLAGS) -c PhysicalModel.cc

PhyN.o: PhysicalModel.cc PhysicalModel.hh
	$(CC) $(CXXFLAGS) -DNO_PNG -c PhysicalModel.cc

Png.o: Png.cc
	$(CC) $(CXXFLAGS) -c Png.cc

Topology.o: Topology.cc
	$(CC) $(CXXFLAGS) -c Topology.cc

Node.o: Node.cc Node.hh
	$(CC) $(CXXFLAGS) -c Node.cc

clean:
	rm -rf *.o core 

eclean:
	rm -rf *~


