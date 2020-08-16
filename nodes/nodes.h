#ifndef NODES_H
#define NODES_H

#include "../interpreter/interpreter.h"
#include "../lexer/token.h"

#include <iostream>
#include <string>
#include <vector>

enum NodeType
{
	NUMBER,
	STRINGNODE,
	BINOP,
	UNARYOP,
	LIST,
	NOOP
};

const std::string NODETYPES[] = 
{
	"NUMBER", "STRING", "BINOP", "UNARYOP", "LIST", "NOOP"
};

class Node 
{
private:
	NodeType type;
	Position start, end;
public:
	Node(NodeType n, Position s, Position e);
	virtual void print(std::ostream& os) const = 0;
	virtual RTResult visit(Context* context);

	NodeType get_type() const;
	Position get_start() const;
	Position get_end() const;
};

std::ostream& operator<<(std::ostream& os, const Node &node);

class NumberNode : public Node
{
private:
	Token* number;
public:
	NumberNode(Token* tok, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class StringNode : public Node
{
private:
	Token* value;
public:
	StringNode(Token* tok, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class BinOpNode : public Node
{
private:
	Token* op;
	Node* left;
	Node* right;
public:
	BinOpNode(Token* tok, Node* l, Node* r, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class UnaryOpNode : public Node
{
private:
	Token* op;
	Node* child;
public:
	UnaryOpNode(Token* tok, Node* c, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class ListNode : public Node
{
private:
	std::vector<Node*> elements;
public:
	ListNode(std::vector<Node*>& elems, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class NoOpNode : public Node
{
public:
	NoOpNode(Position s, Position e);
	void print(std::ostream& os) const;
};
#endif