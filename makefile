kalman: main.o tracers.o token.o lexer.o nodes.o parser.o values.o interpreter.o
	g++ -std=c++11 -o kalman main.o tracers.o token.o lexer.o nodes.o parser.o values.o interpreter.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp 

tracers.o : tracers/position.o tracers/errors.o tracers/symbol_table.o \
			tracers/context.o
	ld -r -o tracers.o tracers/position.o tracers/errors.o \
		tracers/symbol_table.o tracers/context.o

position.o: tracers/position.cpp
	g++ -std=c++11 -c tracers/position.cpp 

errors.o : tracers/errors.cpp
	g++ -std=c++11 -c tracers/errors.cpp

symbol_table.o : tracers/symbol_table.cpp
	g++ -std=c++11 -c tracers/symbol_table.cpp

context.o : tracers/context.cpp
	g++ -std=c++11 -c tracers/context.cpp

token.o: lexer/token.cpp
	g++ -std=c++11 -c lexer/token.cpp 

lexer.o : lexer/lexer.cpp
	g++ -std=c++11 -c lexer/lexer.cpp

nodes.o : nodes/nodes_base.o nodes/nodes_noop.o nodes/nodes_number.o \
			nodes/nodes_binary.o nodes/nodes_unary.o nodes/nodes_list.o nodes/nodes_string.o
	ld -r -o nodes.o nodes/nodes_base.o nodes/nodes_noop.o nodes/nodes_number.o \
			nodes/nodes_binary.o nodes/nodes_unary.o nodes/nodes_list.o nodes/nodes_string.o

nodes_base.o : nodes/nodes_base.cpp
	g++ -std=c++11 -c nodes/nodes_base.cpp

nodes_noop.o : nodes/nodes_noop.cpp
	g++ -std=c++11 -c nodes/nodes_noop.cpp

nodes_number.o : nodes/nodes_number.cpp
	g++ -std=c++11 -c nodes/nodes_number.cpp

nodes_string.o : nodes/nodes_string.cpp
	g++ -std=c++11 -c nodes/nodes_string.cpp

nodes_binary.o : nodes/nodes_binary.cpp
	g++ -std=c++11 -c nodes/nodes_binary.cpp

nodes_unary.o : nodes/nodes_unary.cpp
	g++ -std=c++11 -c nodes/nodes_unary.cpp

nodes_list.o : nodes/nodes_list.cpp
	g++ -std=c++11 -c nodes/nodes_list.cpp

parser.o : parser/parser_base.o parser/statement.o parser/expressions.o parser/terms.o
	ld -r -o parser.o parser/parser_base.o parser/statement.o parser/expressions.o parser/terms.o

parser_base.o : parser/parser_base.cpp
	g++ -std=c++11 -c parser/parser_base.cpp

statement.o : parser/statement.cpp
	g++ -std=c++11 -c parser/statement.cpp

expressions.o : parser/expressions.cpp
	g++ -std=c++11 -c parser/expressions.cpp

terms.o : parser/terms.cpp
	g++ -std=c++11 -c parser/terms.cpp

values.o : values/values_base.o values/values_null.o values/values_integer.o \
			values/values_double.o values/values_list.o values/values_string.o
	ld -r -o values.o values/values_base.o values/values_null.o \
			values/values_integer.o values/values_double.o values/values_list.o \
			values/values_string.o

values_base.o : values/values_base.cpp
	g++ -std=c++11 -c values/values_base.cpp

values_null.o : values/values_null.cpp
	g++ -std=c++11 -c values/values_null.cpp

values_integer.o : values/values_integer.cpp
	g++ -std=c++11 -c values/values_integer.cpp

values_double.o : values/values_double.cpp
	g++ -std=c++11 -c values/values_double.cpp

values_list.o : values/values_list.cpp
	g++ -std=c++11 -c values/values_list.cpp

values_string.o : values/values_string.cpp
	g++ -std=c++11 -c values/values_string.cpp
	
interpreter.o : interpreter/interpreter.cpp
	g++ -std=c++11 -c interpreter/interpreter.cpp

clean:
	rm *.o */*.o kalman 