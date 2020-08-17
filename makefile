kalman: main.o tracers.o token.o lexer.o nodes.o parser.o values.o interpreter.o builtin_functions.o
	g++ -std=c++11 -o kalman main.o tracers.o token.o lexer.o nodes.o parser.o values.o \
					interpreter.o builtin_functions.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp 

tracers.o : tracers/position.o tracers/errors.o tracers/symbol_table.o tracers/context.o
	ld -r -o tracers.o tracers/position.o tracers/errors.o tracers/symbol_table.o tracers/context.o

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

nodes.o : nodes/nodes_base.o nodes/nodes_literal.o nodes/nodes_operation.o \
			nodes/nodes_sequence.o nodes/nodes_condition.o nodes/nodes_loop.o nodes/nodes_var.o \
			nodes/nodes_functions.o nodes/nodes_index.o
	ld -r -o nodes.o nodes/nodes_base.o nodes/nodes_literal.o nodes/nodes_operation.o \
			nodes/nodes_sequence.o nodes/nodes_condition.o nodes/nodes_loop.o nodes/nodes_var.o \
			nodes/nodes_functions.o nodes/nodes_index.o

nodes_base.o : nodes/nodes_base.cpp
	g++ -std=c++11 -c nodes/nodes_base.cpp

nodes_literal.o : nodes/nodes_literal.cpp
	g++ -std=c++11 -c nodes/nodes_literal.cpp

nodes_operation.o : nodes/nodes_operation.cpp
	g++ -std=c++11 -c nodes/nodes_operation.cpp

nodes_sequence.o : nodes/nodes_sequence.cpp
	g++ -std=c++11 -c nodes/nodes_sequence.cpp

nodes_condition.o : nodes/nodes_condition.cpp
	g++ -std=c++11 -c nodes/nodes_condition.cpp

nodes_loop.o : nodes/nodes_loop.cpp
	g++ -std=c++11 -c nodes/nodes_loop.cpp

nodes_var.o : nodes/nodes_var.cpp
	g++ -std=c++11 -c nodes/nodes_var.cpp

nodes_functions.o : nodes/nodes_functions.cpp
	g++ -std=c++11 -c nodes/nodes_functions.cpp

nodes_index.o : nodes/nodes_index.cpp
	g++ -std=c++11 -c nodes/nodes_index.cpp

parser.o : parser/parser_base.o parser/statements.o parser/expressions.o \
		 	parser/terms.o parser/atom_exprs.o
	ld -r -o parser.o parser/parser_base.o parser/statements.o parser/expressions.o \
			parser/terms.o parser/atom_exprs.o

parser_base.o : parser/parser_base.cpp
	g++ -std=c++11 -c parser/parser_base.cpp

statements.o : parser/statements.cpp
	g++ -std=c++11 -c parser/statements.cpp

expressions.o : parser/expressions.cpp
	g++ -std=c++11 -c parser/expressions.cpp

terms.o : parser/terms.cpp
	g++ -std=c++11 -c parser/terms.cpp

atom_exprs.o : parser/atom_exprs.cpp
	g++ -std=c++11 -c parser/atom_exprs.cpp

values.o : values/values_base.o values/values_null.o values/values_integer.o \
			values/values_double.o values/values_list.o values/values_string.o \
			values/values_function.o values/values_builtinfunc.o
	ld -r -o values.o values/values_base.o values/values_null.o \
			values/values_integer.o values/values_double.o values/values_list.o \
			values/values_string.o values/values_function.o values/values_builtinfunc.o

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

values_function.o : values/values_function.cpp
	g++ -std=c++11 -c values/values_function.cpp

values_builtinfunc.o : values/values_builtinfunc.cpp
	g++ -std=c++11 -c values/values_builtinfunc.cpp

interpreter.o : interpreter/interpreter.cpp
	g++ -std=c++11 -c interpreter/interpreter.cpp

builtin_functions.o : interpreter/builtin_functions.cpp
	g++ -std=c++11 -c interpreter/builtin_functions.cpp

clean:
	rm *.o */*.o kalman 