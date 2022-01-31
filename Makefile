myTar: o_myTar.o
	g++ o_myTar.o -o myTar

o_myTar.o: myTar.cpp
	g++ -c myTar.cpp -o o_myTar.o
