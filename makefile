kalman: main.o position.o token.o values.o symbol_table.o context.o errors.o lexer.o interpreter.o nodes.o parser.o 
	g++ -std=c++11 main.o position.o token.o values.o symbol_table.o \
	context.o errors.o lexer.o interpreter.o nodes.o parser.o -o kalman 

main.o: main.cpp
	g++ -std=c++11 -c main.cpp 

position.o: position.cpp
	g++ -std=c++11 -c position.cpp 

errors.o : errors.cpp
	g++ -std=c++11 -c errors.cpp

token.o: token.cpp
	g++ -std=c++11 -c token.cpp 

symbol_table.o : symbol_table.cpp
	g++ -std=c++11 -c symbol_table.cpp

context.o : context.cpp
	g++ -std=c++11 -c context.cpp

lexer.o : lexer.cpp
	g++ -std=c++11 -c lexer.cpp

parser.o : parser.cpp
	g++ -std=c++11 -c parser.cpp

interpreter.o : interpreter.cpp
	g++ -std=c++11 -c interpreter.cpp

nodes.o : nodes/nodes_base.o nodes/nodes_noop.o nodes/nodes_number.o \
			nodes/nodes_binary.o nodes/nodes_unary.o nodes/nodes_list.o
	ld -r -o nodes.o nodes/nodes_base.o nodes/nodes_noop.o nodes/nodes_number.o \
			nodes/nodes_binary.o nodes/nodes_unary.o nodes/nodes_list.o

nodes_base.o : nodes/nodes_base.cpp
	g++ -std=c++11 -c nodes/nodes_base.cpp

nodes_noop.o : nodes/nodes_noop.cpp
	g++ -std=c++11 -c nodes/nodes_noop.cpp

nodes_number.o : nodes/nodes_number.cpp
	g++ -std=c++11 -c nodes/nodes_number.cpp

nodes_binary.o : nodes/nodes_binary.cpp
	g++ -std=c++11 -c nodes/nodes_binary.cpp

nodes_unary.o : nodes/nodes_unary.cpp
	g++ -std=c++11 -c nodes/nodes_unary.cpp

nodes_list.o : nodes/nodes_list.cpp
	g++ -std=c++11 -c nodes/nodes_list.cpp

values.o : values/values_base.o values/values_null.o values/values_integer.o values/values_double.o 
	ld -r -o values.o values/values_base.o values/values_null.o values/values_integer.o values/values_double.o 

values_base.o : values/values_base.cpp
	g++ -std=c++11 -c values/values_base.cpp

values_null.o : values/values_null.cpp
	g++ -std=c++11 -c values/values_null.cpp

values_integer.o : values/values_integer.cpp
	g++ -std=c++11 -c values/values_integer.cpp

values_double.o : values/values_double.cpp
	g++ -std=c++11 -c values/values_double.cpp

clean:
	rm *.o nodes/*.o values/*.o kalman 