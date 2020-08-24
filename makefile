kalman: program.o objects.o contexts.o diagnostics.o syntax.o evaluator.o builtin-functions.o
	g++ -O2 -march=native -Wall -std=c++17 -o kalman program.o objects.o contexts.o diagnostics.o syntax.o evaluator.o builtin-functions.o

program.o: program.cpp 
	g++ -O2 -march=native -Wall -std=c++17 -c program.cpp 

objects.o: boolean-object.o integer-object.o double-object.o \
		string-object.o list-object.o function-object.o none-object.o object-helpers.o \
		base-object.o
	ld -r -o objects.o boolean-object.o integer-object.o double-object.o \
		string-object.o list-object.o function-object.o none-object.o object-helpers.o \
		base-object.o

base-object.o: Objects/base-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/base-object.cpp

boolean-object.o: Objects/boolean-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/boolean-object.cpp

integer-object.o: Objects/integer-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/integer-object.cpp

double-object.o: Objects/double-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/double-object.cpp

string-object.o: Objects/string-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/string-object.cpp

list-object.o: Objects/list-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/list-object.cpp

function-object.o: Objects/function-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/function-object.cpp

none-object.o: Objects/none-object.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/none-object.cpp

object-helpers.o: Objects/object-helpers.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Objects/object-helpers.cpp

contexts.o: context.o symbol-table.o 
	ld -r -o contexts.o context.o symbol-table.o 

context.o: Contexts/context.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Contexts/context.cpp

symbol-table.o: Contexts/symbol-table.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Contexts/symbol-table.cpp

diagnostics.o: diagnostic.o diagnostic-bag.o
	ld -r -o diagnostics.o diagnostic.o diagnostic-bag.o

diagnostic.o: Diagnostics/diagnostic.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Diagnostics/diagnostic.cpp

diagnostic-bag.o: Diagnostics/diagnostic-bag.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Diagnostics/diagnostic-bag.cpp

syntax.o: lexer.o syntax-facts.o syntax-helpers.o syntax-node.o syntax-token.o syntax-expressions.o \
			parser.o
	ld -r -o syntax.o lexer.o syntax-facts.o syntax-helpers.o syntax-node.o syntax-token.o \
			syntax-expressions.o parser.o

lexer.o: Syntax/lexer.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/lexer.cpp

parser.o: Syntax/parser.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/parser.cpp

syntax-facts.o: Syntax/syntax-facts.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/syntax-facts.cpp

syntax-helpers.o: Syntax/syntax-helpers.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/syntax-helpers.cpp

syntax-node.o: Syntax/syntax-node.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/syntax-node.cpp

syntax-token.o: Syntax/syntax-token.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/syntax-token.cpp

syntax-expressions.o: binary-syntax.o func-call-syntax.o func-define-syntax.o for-syntax.o if-syntax.o \
			literal-syntax.o sequence-syntax.o unary-syntax.o var-access-syntax.o var-assign-syntax.o \
			var-declare-syntax.o while-syntax.o index-syntax.o none-syntax.o \
			return-syntax.o continue-syntax.o break-syntax.o
	ld -r -o syntax-expressions.o binary-syntax.o func-call-syntax.o func-define-syntax.o for-syntax.o if-syntax.o \
			literal-syntax.o sequence-syntax.o unary-syntax.o var-access-syntax.o var-assign-syntax.o \
			var-declare-syntax.o while-syntax.o index-syntax.o none-syntax.o \
			return-syntax.o continue-syntax.o break-syntax.o

binary-syntax.o: Syntax/Expressions/binary-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/binary-syntax.cpp

func-call-syntax.o: Syntax/Expressions/func-call-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/func-call-syntax.cpp

func-define-syntax.o: Syntax/Expressions/func-define-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/func-define-syntax.cpp

for-syntax.o: Syntax/Expressions/for-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/for-syntax.cpp

if-syntax.o: Syntax/Expressions/if-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/if-syntax.cpp

literal-syntax.o: Syntax/Expressions/literal-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/literal-syntax.cpp

sequence-syntax.o: Syntax/Expressions/sequence-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/sequence-syntax.cpp

unary-syntax.o: Syntax/Expressions/unary-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/unary-syntax.cpp

var-access-syntax.o: Syntax/Expressions/var-access-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/var-access-syntax.cpp

var-assign-syntax.o: Syntax/Expressions/var-assign-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/var-assign-syntax.cpp

var-declare-syntax.o: Syntax/Expressions/var-declare-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/var-declare-syntax.cpp

while-syntax.o: Syntax/Expressions/while-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/while-syntax.cpp

index-syntax.o: Syntax/Expressions/index-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/index-syntax.cpp

none-syntax.o: Syntax/Expressions/none-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/none-syntax.cpp

return-syntax.o: Syntax/Expressions/return-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/return-syntax.cpp

continue-syntax.o: Syntax/Expressions/continue-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/continue-syntax.cpp

break-syntax.o: Syntax/Expressions/break-syntax.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Syntax/Expressions/break-syntax.cpp

evaluator.o: Evaluator/evaluator.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Evaluator/evaluator.cpp

builtin-functions.o: Evaluator/builtin-functions.cpp
	g++ -O2 -march=native -Wall -std=c++17 -c Evaluator/builtin-functions.cpp

make clean:
	rm *.o 