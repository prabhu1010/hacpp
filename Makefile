TableMaker:	TableMaker.o
		g++ -std=c++11 TableMaker.o -o TableMaker
		g++ -std=c++11 TableUpdater.o -o TableUpdater
TableUpdater:	TableUpdater.o
		g++ -std=c++11 TableUpdater.o -o TableUpdater
TableMaker.o:	TableMaker.cc
		g++ -c -std=c++11 TableMaker.cc -o TableMaker.o
		g++ -c -std=c++11 TableUpdater.cc -o TableUpdater.o
TableUpdater.o:	TableUpdater.cc
		g++ -c -std=c++11 TableUpdater.cc -o TableUpdater.o
clean:		
		rm TableMaker.o TableMaker TableUpdater.o TableUpdater
