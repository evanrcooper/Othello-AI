othello.exe: main.o board.o alphabeta.o heuristic.o timeKeep.o
	g++ -o othello.exe main.o board.o alphabeta.o heuristic.o timeKeep.o

main.o: main.cpp board.cpp board.h alphabeta.cpp alphabeta.h heuristic.cpp heuristic.h
	g++ -c main.cpp

board.o: board.cpp board.h
	g++ -c board.cpp

alphabeta.o: alphabeta.cpp alphabeta.h board.cpp board.h heuristic.cpp heuristic.h timeKeep.cpp timeKeep.h
	g++ -c alphabeta.cpp

heuristic.o: heuristic.cpp heuristic.h weights.h
	g++ -c heuristic.cpp

timeKeep.o: timeKeep.cpp timeKeep.h
	g++ -c timeKeep.cpp

debug:
	g++ -g -o othelloDebug.out main.cpp

clean:
	rm -f *.exe *.o *.stackdump *~ *.out *.gch

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
# tar -czf backup-$(shell date --iso=seconds).tar.gz backups

# build and run executable
run:
	make
	./othello.exe

# build executable then clear terminal and run executable
crun:
	make
	clear
	./othello.exe

# deletes temporary files and then rebuilds the executable
refresh:
	make clean
	make