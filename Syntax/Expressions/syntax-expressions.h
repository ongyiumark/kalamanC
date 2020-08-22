#pragma once

#include "../syntax.h"

namespace Syntax
{
    class LiteralExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _literal_token;
        Objects::Object* _value;
    public:
        LiteralExpressionSyntax(SyntaxToken* literal_token, Objects::Object* value);
        LiteralExpressionSyntax(SyntaxToken* literal_token);

        SyntaxKind kind() const;
        Objects::Object* get_object() const;
    };

    class UnaryExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _op_token;
        SyntaxNode* _operand;
    public:
        UnaryExpressionSyntax(SyntaxToken* op_token, SyntaxNode* operand);
        SyntaxKind kind() const;
        
        SyntaxToken* get_op_token() const;
        SyntaxNode* get_operand() const;
    };

    class BinaryExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _left;
        SyntaxToken* _op_token;
        SyntaxNode*_right;
    public:
        BinaryExpressionSyntax(SyntaxNode* left, SyntaxToken* op_token, SyntaxNode* right);
        SyntaxKind kind() const;
        SyntaxNode* get_left() const;
        SyntaxToken* get_op_token() const;
        SyntaxNode* get_right() const;
    };

    class SequenceExpressionSyntax final : public SyntaxNode
    {
    private:
        std::vector<SyntaxNode*> _nodes;
        bool _to_return;
    public:
        SequenceExpressionSyntax(std::vector<SyntaxNode*>& nodes, bool to_return=false);
        SyntaxKind kind() const;    

        int get_nodes_size() const;
        SyntaxNode* get_node(int i) const;
        std::vector<SyntaxNode*> get_nodes() const;
        bool get_to_return() const;
    };

    class WhileExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _condition;
        SyntaxNode* _body;
    public:
        WhileExpressionSyntax(SyntaxNode* condition, SyntaxNode* body);
        SyntaxKind kind() const;

        SyntaxNode* get_condition() const;
        SyntaxNode* get_body() const;
    };

    class ForExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode *_init, *_condition, *_update, *_body;
    public:
        ForExpressionSyntax(SyntaxNode* init, SyntaxNode* condition, SyntaxNode* update, SyntaxNode* body);
        SyntaxKind kind() const;
        SyntaxNode* get_init() const;     
        SyntaxNode* get_condition() const;      
        SyntaxNode* get_update() const;      
        SyntaxNode* get_body() const;            
    };

    class VarDeclareExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _var_keyword;
        SyntaxToken* _identifier;
    public:
        VarDeclareExpressionSyntax(SyntaxToken* var_keyword, SyntaxToken* identifier);
        SyntaxKind kind() const;
        SyntaxToken* get_var_keyword() const;
        SyntaxToken* get_identifier() const;
    };

    class VarAssignExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _identifier;
        SyntaxNode* _value;
    public:
        VarAssignExpressionSyntax(SyntaxToken* identifier, SyntaxNode* value);
        SyntaxKind kind() const;
        SyntaxToken* get_identifier() const;
        SyntaxNode* get_value() const;
    };

    class VarAccessExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _identifier;
    public:
        VarAccessExpressionSyntax(SyntaxToken* identifier);
        SyntaxKind kind() const;
        SyntaxToken* get_identifier() const;
    };

    class IfExpressionSyntax final : public SyntaxNode
    {
    private:
        std::vector<SyntaxNode*> _conditions;
        std::vector<SyntaxNode*> _bodies;
        SyntaxNode* _else_body;
    public:
        IfExpressionSyntax(std::vector<SyntaxNode*>& conditions,std::vector<SyntaxNode*>& bodies, SyntaxNode* else_body);
        SyntaxKind kind() const;
        SyntaxNode* get_else_body() const;
        
        int get_size() const;
        SyntaxNode* get_condition(int i) const;
        SyntaxNode* get_body(int i) const;

        std::vector<SyntaxNode*> get_conditions() const;
        std::vector<SyntaxNode*> get_bodies() const;
    };

    class DefFuncExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _identifier;
        std::vector<SyntaxToken*> _arg_names;
        SyntaxNode* _body;
    public:
        DefFuncExpressionSyntax(SyntaxToken* identifier, std::vector<SyntaxToken*>& arg_names, SyntaxNode* body);

        SyntaxKind kind() const;
        SyntaxToken* get_identifier() const;
        int get_arg_size() const;
        SyntaxToken* get_arg_name(int i) const;
        std::vector<SyntaxToken*> get_arg_names() const;
        SyntaxNode* get_body() const;
    };

    class CallExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken* _identifier;
        std::vector<SyntaxNode*> _args;
    public:
        CallExpressionSyntax(SyntaxToken* identifier, std::vector<SyntaxNode*>& args);
        SyntaxKind kind() const;
        SyntaxToken* get_identifier() const;
        int get_arg_size() const;
        SyntaxNode* get_arg(int i) const;
        std::vector<SyntaxNode*> get_args() const;
    }; 

    class IndexExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _to_access;
        SyntaxNode* _indexer;
    public:
        IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer);
        SyntaxKind kind() const;
        SyntaxNode* get_to_access() const;
        SyntaxNode* get_indexer() const;
    };

    class NoneExpressionSyntax final : public SyntaxNode
    {
    public:
        NoneExpressionSyntax();
        SyntaxKind kind() const;
    };

    class ReturnExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _to_return;
    public:
        ReturnExpressionSyntax(SyntaxNode* to_return);
        SyntaxKind kind() const;
        SyntaxNode* get_to_return() const;
    };

    class ContinueExpressionSyntax final : public SyntaxNode
    {
    public:
        ContinueExpressionSyntax();
        SyntaxKind kind() const;
    };

    class BreakExpressionSyntax final : public SyntaxNode
    {
    public:
        BreakExpressionSyntax();
        SyntaxKind kind() const;
    };

    class Parser final
    {
    private:
        std::vector<SyntaxToken*> _tokens;
        int _position;
        Diagnostics::DiagnosticBag* _diagnostics; 

        SyntaxToken* peek(int offset) const;
        SyntaxToken* current() const;
        SyntaxToken* look_ahead() const;
        SyntaxToken* next_token();
        SyntaxToken* match_token(SyntaxKind kind);

        SyntaxNode* parse_atom();
        SyntaxNode* parse_molecule();
        SyntaxNode* parse_expression(int precedence = 0);
        SyntaxNode* parse_statement();
        SyntaxNode* parse_program(bool sub_program=false);
    public:
        Parser(std::string& text);
        SyntaxNode* parse();
    };
}