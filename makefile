kalman : program.o code-analysis.o 
	g++ -std=c++17 -o kalman program.o code-analysis.o 

program.o: program.cpp
	g++ -std=c++17 -c program.cpp

code-analysis.o : evaluator.o syntax.o binding.o
	ld -r -o code-analysis.o evaluator.o syntax.o binding.o

evaluator.o: CodeAnalysis/evaluator.cpp
	g++ -std=c++17 -c CodeAnalysis/evaluator.cpp

syntax.o: syntax-token.o lexer.o parser.o syntax-helpers.o literal-expression-syntax.o \
					binary-expression-syntax.o unary-expression-syntax.o paren-expression-syntax.o \
					syntax-node.o syntax-tree.o syntax-facts.o
	ld -r -o syntax.o syntax-token.o lexer.o parser.o syntax-helpers.o literal-expression-syntax.o \
			binary-expression-syntax.o unary-expression-syntax.o paren-expression-syntax.o syntax-node.o \
			syntax-tree.o syntax-facts.o

syntax-node.o: CodeAnalysis/Syntax/syntax-node.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/syntax-node.cpp

syntax-token.o: CodeAnalysis/Syntax/syntax-token.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/syntax-token.cpp

lexer.o: CodeAnalysis/Syntax/lexer.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/lexer.cpp

parser.o: CodeAnalysis/Syntax/parser.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/parser.cpp

syntax-helpers.o: CodeAnalysis/Syntax/syntax-helpers.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/syntax-helpers.cpp

literal-expression-syntax.o: CodeAnalysis/Syntax/literal-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/literal-expression-syntax.cpp

binary-expression-syntax.o: CodeAnalysis/Syntax/binary-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/binary-expression-syntax.cpp

unary-expression-syntax.o: CodeAnalysis/Syntax/unary-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/unary-expression-syntax.cpp

paren-expression-syntax.o: CodeAnalysis/Syntax/paren-expression-syntax.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/paren-expression-syntax.cpp

syntax-tree.o: CodeAnalysis/Syntax/syntax-tree.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/syntax-tree.cpp

syntax-facts.o: CodeAnalysis/Syntax/syntax-facts.cpp
	g++ -std=c++17 -c CodeAnalysis/Syntax/syntax-facts.cpp

binding.o : binder.o bound-binary-expression.o bound-literal-expression.o bound-unary-expression.o binding-helpers.o \
		bound-unary-operator.o bound-binary-operator.o
	ld -r -o binding.o binder.o bound-binary-expression.o bound-literal-expression.o bound-unary-expression.o \
		binding-helpers.o bound-unary-operator.o bound-binary-operator.o

binder.o : CodeAnalysis/Binding/binder.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/binder.cpp

bound-binary-expression.o : CodeAnalysis/Binding/bound-binary-expression.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/bound-binary-expression.cpp

bound-literal-expression.o : CodeAnalysis/Binding/bound-literal-expression.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/bound-literal-expression.cpp

bound-unary-expression.o : CodeAnalysis/Binding/bound-unary-expression.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/bound-unary-expression.cpp

bound-unary-operator.o : CodeAnalysis/Binding/bound-unary-operator.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/bound-unary-operator.cpp

bound-binary-operator.o : CodeAnalysis/Binding/bound-binary-operator.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/bound-binary-operator.cpp

binding-helpers.o : CodeAnalysis/Binding/binding-helpers.cpp
	g++ -std=c++17 -c CodeAnalysis/Binding/binding-helpers.cpp

make clean:
	rm *.o kalman