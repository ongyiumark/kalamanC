kalman : program.o code-analysis.o 
	g++ -std=c++17 -o kalman program.o code-analysis.o 

program.o: program.cpp
	g++ -std=c++17 -c program.cpp

code-analysis.o : syntax-token.o lexer.o helpers.o
	ld -r -o code-analysis.o syntax-token.o lexer.o helpers.o

syntax-token.o: CodeAnalysis/syntax-token.cpp
	g++ -std=c++17 -c CodeAnalysis/syntax-token.cpp

lexer.o: CodeAnalysis/lexer.cpp
	g++ -std=c++17 -c CodeAnalysis/lexer.cpp

helpers.o: CodeAnalysis/helpers.cpp
	g++ -std=c++17 -c CodeAnalysis/helpers.cpp

make clean:
	rm *.o kalman