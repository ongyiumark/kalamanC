kalman: main.o position.o token.o values.o symbol_table.o context.o errors.o lexer.o
	g++ -std=c++11 main.o position.o token.o values.o symbol_table.o context.o errors.o lexer.o -o kalman 

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

clean:
	rm *.o kalman