kalman: program.o objects.o diagnostics.o
	g++ -g -std=c++17 -o kalman program.o objects.o diagnostics.o

program.o: program.cpp 
	g++ -g -std=c++17 -c program.cpp 

objects.o: boolean-object.o integer-object.o double-object.o \
		string-object.o list-object.o function-object.o none-object.o object-helpers.o
	ld -r -o objects.o boolean-object.o integer-object.o double-object.o \
		string-object.o list-object.o function-object.o none-object.o object-helpers.o 

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

none-object.o: Objects/none-object.cpp
	g++ -g -std=c++17 -c Objects/none-object.cpp

object-helpers.o: Objects/object-helpers.cpp
	g++ -g -std=c++17 -c Objects/object-helpers.cpp

diagnostics.o: context.o symbol_table.o position.o diagnostic.o diagnostic_bag.o
	ld -r -o diagnostics.o context.o symbol_table.o position.o diagnostic.o diagnostic_bag.o

context.o: Diagnostics/context.cpp
	g++ -g -std=c++17 -c Diagnostics/context.cpp

symbol_table.o: Diagnostics/symbol_table.cpp
	g++ -g -std=c++17 -c Diagnostics/symbol_table.cpp

position.o: Diagnostics/position.cpp
	g++ -g -std=c++17 -c Diagnostics/position.cpp

diagnostic.o: Diagnostics/diagnostic.cpp
	g++ -g -std=c++17 -c Diagnostics/diagnostic.cpp

diagnostic_bag.o: Diagnostics/diagnostic_bag.cpp
	g++ -g -std=c++17 -c Diagnostics/diagnostic_bag.cpp

make clean:
	rm *.o 