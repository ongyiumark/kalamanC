kalman: main.o position.o token.o values.o symbol_table.o context.o errors.o lexer.o interpreter.o nodes.o parser.o 
	g++ -std=c++11 main.o position.o token.o values.o symbol_table.o \
	context.o errors.o lexer.o interpreter.o nodes.o parser.o -o kalman 

main.o: main.cpp
	g++ -std=c++11 -c main.cpp 

position.o: position.cpp
	g++ -std=c++11 -c position.cpp 

token.o: token.cpp
	g++ -std=c++11 -c token.cpp 

values.o : values.cpp
	g++ -std=c++11 -c values.cpp

symbol_table.o : symbol_table.cpp
	g++ -std=c++11 -c symbol_table.cpp

context.o : context.cpp
	g++ -std=c++11 -c context.cpp

errors.o : errors.cpp
	g++ -std=c++11 -c errors.cpp

lexer.o : lexer.cpp
	g++ -std=c++11 -c lexer.cpp

interpreter.o : interpreter.cpp
	g++ -std=c++11 -c interpreter.cpp

nodes.o : nodes.cpp
	g++ -std=c++11 -c nodes.cpp

parser.o : parser.cpp
	g++ -std=c++11 -c parser.cpp

clean:
	rm *.o kalman