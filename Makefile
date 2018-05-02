main: main.o Node.o
		g++ -o main main.o Node.o
Node.o:Node.cpp
		g++ -c Node.cpp

	
clean:
	clear ; rm *.o ; rm *.txt
