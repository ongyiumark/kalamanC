#ifndef NODES_H
#define NODES_H

#include "../interpreter/interpreter.h"
#include "../lexer/token.h"
#include "../values/values.h"

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
	SEQUENCE,
	CONDITION,
	FORNODE,
	WHILENODE,
	BREAKNODE,
	CONTNODE,
	VARDECL,
	VARASSIGN,
	VARACCESS,
	NOOP
};

const std::string NODETYPES[] = 
{
	"NUMBER", "STRING", "BINOP", "UNARYOP", "LIST", "SEQUENCE", 
	"CONDITION", "FORNODE", "WHILENODE", "BREAKNODE", "CONTNODE", 
	"VARDECL", "VARASSIGN", "VARACCESS", "NOOP"
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

class SequenceNode : public Node
{
private:
	std::vector<Node*> elements;
public:
	SequenceNode(std::vector<Node*>& elems, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class NoOpNode : public Node
{
public:
	NoOpNode(Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class ConditionNode : public Node
{
private:
	std::vector<Node*> cases;
	std::vector<Node*> statements;
	Node* else_case;
public:
	ConditionNode(std::vector<Node*>& cs, std::vector<Node*>& stms, Node* ecs, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class ForLoopNode : public Node
{
private:
	Node* init;
	Node* condition;
	Node* update;
	Node* body;
public:
	ForLoopNode(Node* i, Node * c, Node* u, Node* b, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class WhileLoopNode : public Node
{
private:
	Node* condition;
	Node* body;
public:
	WhileLoopNode(Node * c, Node* b, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class BreakNode : public Node
{
public:
	BreakNode(Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class ContinueNode : public Node
{
public:
	ContinueNode(Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class VarDeclareNode : public Node
{
private:
	ValueType type;
	Token* identifier;
public:
	VarDeclareNode(ValueType t, Token* id, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class VarAssignNode : public Node
{
private:
	Token* identifier;
	Node* child;
public:
	VarAssignNode(Token* id, Node* c, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

class VarAccessNode : public Node
{
private:
	Token* identifier;
public:
	VarAccessNode(Token* id, Position s, Position e);
	void print(std::ostream& os) const;
	RTResult visit(Context* context);
};

#endif