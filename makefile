kalman: program.o objects.o contexts.o diagnostics.o syntax.o
	g++ -g -std=c++17 -o kalman program.o objects.o contexts.o diagnostics.o syntax.o

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

contexts.o: context.o symbol_table.o 
	ld -r -o contexts.o context.o symbol_table.o 

context.o: Contexts/context.cpp
	g++ -g -std=c++17 -c Contexts/context.cpp

symbol_table.o: Contexts/symbol_table.cpp
	g++ -g -std=c++17 -c Contexts/symbol_table.cpp

diagnostics.o: position.o diagnostic.o diagnostic_bag.o
	ld -r -o diagnostics.o position.o diagnostic.o diagnostic_bag.o

position.o: Diagnostics/position.cpp
	g++ -g -std=c++17 -c Diagnostics/position.cpp

diagnostic.o: Diagnostics/diagnostic.cpp
	g++ -g -std=c++17 -c Diagnostics/diagnostic.cpp

diagnostic_bag.o: Diagnostics/diagnostic_bag.cpp
	g++ -g -std=c++17 -c Diagnostics/diagnostic_bag.cpp

syntax.o: lexer.o syntax-facts.o syntax-helpers.o syntax-node.o syntax-token.o syntax-expressions.o \
			parser.o
	ld -r -o syntax.o lexer.o syntax-facts.o syntax-helpers.o syntax-node.o syntax-token.o \
			syntax-expressions.o parser.o

lexer.o: Syntax/lexer.cpp
	g++ -g -std=c++17 -c Syntax/lexer.cpp

parser.o: Syntax/parser.cpp
	g++ -g -std=c++17 -c Syntax/parser.cpp

syntax-facts.o: Syntax/syntax-facts.cpp
	g++ -g -std=c++17 -c Syntax/syntax-facts.cpp

syntax-helpers.o: Syntax/syntax-helpers.cpp
	g++ -g -std=c++17 -c Syntax/syntax-helpers.cpp

syntax-node.o: Syntax/syntax-node.cpp
	g++ -g -std=c++17 -c Syntax/syntax-node.cpp

syntax-token.o: Syntax/syntax-token.cpp
	g++ -g -std=c++17 -c Syntax/syntax-token.cpp

syntax-expressions.o: binary-syntax.o call-syntax.o deffunc-syntax.o for-syntax.o if-syntax.o \
			literal-syntax.o sequence-syntax.o unary-syntax.o var-access-syntax.o var-assign-syntax.o \
			var-declare-syntax.o while-syntax.o index-syntax.o none-syntax.o
	ld -r -o syntax-expressions.o binary-syntax.o call-syntax.o deffunc-syntax.o for-syntax.o if-syntax.o \
			literal-syntax.o sequence-syntax.o unary-syntax.o var-access-syntax.o var-assign-syntax.o \
			var-declare-syntax.o while-syntax.o index-syntax.o none-syntax.o

binary-syntax.o: Syntax/Expressions/binary-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/binary-syntax.cpp

call-syntax.o: Syntax/Expressions/call-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/call-syntax.cpp

deffunc-syntax.o: Syntax/Expressions/deffunc-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/deffunc-syntax.cpp

for-syntax.o: Syntax/Expressions/for-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/for-syntax.cpp

if-syntax.o: Syntax/Expressions/if-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/if-syntax.cpp

literal-syntax.o: Syntax/Expressions/literal-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/literal-syntax.cpp

sequence-syntax.o: Syntax/Expressions/sequence-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/sequence-syntax.cpp

unary-syntax.o: Syntax/Expressions/unary-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/unary-syntax.cpp

var-access-syntax.o: Syntax/Expressions/var-access-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/var-access-syntax.cpp

var-assign-syntax.o: Syntax/Expressions/var-assign-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/var-assign-syntax.cpp

var-declare-syntax.o: Syntax/Expressions/var-declare-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/var-declare-syntax.cpp

while-syntax.o: Syntax/Expressions/while-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/while-syntax.cpp

index-syntax.o: Syntax/Expressions/index-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/index-syntax.cpp

none-syntax.o: Syntax/Expressions/none-syntax.cpp
	g++ -g -std=c++17 -c Syntax/Expressions/none-syntax.cpp

make clean:
	rm *.o 