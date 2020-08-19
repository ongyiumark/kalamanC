kalman : program.o code-analysis.o 
	g++ -std=c++17 -o kalman program.o code-analysis.o 

program.o: program.cpp
	g++ -std=c++17 -c program.cpp

code-analysis.o : syntax-token.o lexer.o parser.o helpers.o literal-expression-syntax.o \
					binary-expression-syntax.o unary-expression-syntax.o paren-expression-syntax.o \
					syntax-node.o syntax-tree.o evaluator.o syntax-facts.o
	ld -r -o code-analysis.o syntax-token.o lexer.o parser.o helpers.o literal-expression-syntax.o \
			binary-expression-syntax.o unary-expression-syntax.o paren-expression-syntax.o syntax-node.o \
			syntax-tree.o evaluator.o syntax-facts.o

syntax-node.o: CodeAnalysis/syntax-node.cpp
	g++ -std=c++17 -c CodeAnalysis/syntax-node.cpp

syntax-token.o: CodeAnalysis/syntax-token.cpp
	g++ -std=c++17 -c CodeAnalysis/syntax-token.cpp

lexer.o: CodeAnalysis/lexer.cpp
	g++ -std=c++17 -c CodeAnalysis/lexer.cpp

parser.o: CodeAnalysis/parser.cpp
	g++ -std=c++17 -c CodeAnalysis/parser.cpp

helpers.o: CodeAnalysis/helpers.cpp
	g++ -std=c++17 -c CodeAnalysis/helpers.cpp

literal-expression-syntax.o: CodeAnalysis/literal-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/literal-expression-syntax.cpp

binary-expression-syntax.o: CodeAnalysis/binary-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/binary-expression-syntax.cpp

unary-expression-syntax.o: CodeAnalysis/unary-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/unary-expression-syntax.cpp

paren-expression-syntax.o: CodeAnalysis/paren-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/paren-expression-syntax.cpp

syntax-tree.o: CodeAnalysis/syntax-tree.cpp
	g++ -std=c++17 -c CodeAnalysis/syntax-tree.cpp

syntax-facts.o: CodeAnalysis/syntax-facts.cpp
	g++ -std=c++17 -c CodeAnalysis/syntax-facts.cpp

evaluator.o: CodeAnalysis/evaluator.cpp
	g++ -std=c++17 -c CodeAnalysis/evaluator.cpp

make clean:
	rm *.o kalman