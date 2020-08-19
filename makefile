kalman : main.o test.o
	g++ -std=c++11 -o kalman main.o test.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

test.o: test.cpp
	g++ -std=c++11 -c test.cpp

make clean:
	rm *.o kalman