CC = g++
LD = g++

CFLAGS = -c -pedantic -std=c++11 -Wall -fno-elide-constructors
LFLAGS = -pedantic -Wall

OBJS = p4_main.o Track_base.o Island.o Ship.o Tanker.o Warship.o Cruiser.o Ship_factory.o Model.o Controller.o Utility.o
PROG = p4exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p4_main.o: p4_main.cpp Model.h Controller.h
	$(CC) $(CFLAGS) p4_main.cpp

Island.o: Island.cpp Island.h Model.h
	$(CC) $(CFLAGS) Island.cpp

Ship.o: Ship.cpp Ship.h Island.h Utility.h Model.h
	$(CC) $(CFLAGS) Ship.cpp

Tanker.o: Tanker.cpp Tanker.h Island.h
	$(CC) $(CFLAGS) Tanker.cpp Ship.cpp

Warship.o: Warship.cpp Warship.h Island.h
	$(CC) $(CFLAGS) Warship.cpp Ship.cpp

Cruiser.o: Cruiser.cpp Cruiser.h Island.h
	$(CC) $(CFLAGS) Cruiser.cpp Warship.cpp

Ship_factory.o: Ship_factory.cpp Ship_factory.h
	$(CC) $(CFLAGS) Ship_factory.cpp 

Model.o: Model.cpp Model.h Island
	$(CC) $(CFLAGS) Model.cpp Island.cpp Ship.cpp View.cpp

Controller.o: Controller.cpp Controller.h 
	$(CC) $(CFLAGS) Controller.cpp Model.cpp Island.cpp Ship.cpp View.cpp Geometry.cpp

Utility.o: Utility.cpp Utility.h
	$(CC) $(CFLAGS) Utility.cpp

Track_base.o: Track_base.cpp Track_base.h
	$(CC) $(CFLAGS) Track_base.cpp Geometry.cpp Navigation.cpp	

clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe