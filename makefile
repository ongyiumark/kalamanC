kalman: program.o objects.o
	g++ -g -std=c++17 -o kalman program.o objects.o

program.o: program.cpp 
	g++ -g -std=c++17 -c program.cpp 

objects.o: boolean-object.o integer-object.o double-object.o \
		string-object.o list-object.o function-object.o object-helpers.o
	ld -r -o objects.o boolean-object.o integer-object.o double-object.o \
		string-object.o list-object.o function-object.o object-helpers.o

boolean-object.o: Objects/boolean-object.cpp
	g++ -g -std=c++17 -c Objects/boolean-object.cpp

integer-object.o: Objects/integer-object.cpp
	g++ -g -std=c++17 -c Objects/integer-object.cpp

double-object.o: Objects/double-object.cpp
	g++ -g -std=c++17 -c Objects/double-object.cpp

string-object.o: Objects/string-object.cpp
	g++ -g -std=c++17 -c Objects/string-object.cpp

list-object.o: Objects/list-object.cpp
	g++ -g -std=c++17 -c Objects/list-object.cpp

function-object.o: Objects/function-object.cpp
	g++ -g -std=c++17 -c Objects/function-object.cpp

object-helpers.o: Objects/object-helpers.cpp
	g++ -g -std=c++17 -c Objects/object-helpers.cpp

make clean:
	rm *.o 