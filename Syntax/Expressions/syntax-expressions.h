#pragma once

#include "../syntax.h"

namespace Syntax
{
    // Refer to literal-syntax.cpp.
    class LiteralExpressionSyntax final : public SyntaxNode
    {
    private:
        Objects::Object* _value;
    public:
        LiteralExpressionSyntax(Objects::Object* value, Diagnostics::Position pos);
        ~LiteralExpressionSyntax();

        SyntaxKind kind() const;
        Objects::Object* get_object();
    };

    // Refer to unary-syntax.cpp.
    class UnaryExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _op_token;
        SyntaxNode* _operand;
    public:
        UnaryExpressionSyntax(SyntaxToken op_token, SyntaxNode* operand, Diagnostics::Position pos);
        ~UnaryExpressionSyntax();

        SyntaxKind kind() const;
        
        SyntaxToken* get_op_token();
        SyntaxNode* get_operand();
    };

    // Refer to binary-syntax.cpp.
    class BinaryExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _left;
        SyntaxToken _op_token;
        SyntaxNode*_right;
    public:
        BinaryExpressionSyntax(SyntaxNode* left, SyntaxToken op_token, SyntaxNode* right, Diagnostics::Position pos);
        ~BinaryExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxNode* get_left();
        SyntaxToken* get_op_token();
        SyntaxNode* get_right();
    };

    // Refer to sequence-syntax.cpp.
    class SequenceExpressionSyntax final : public SyntaxNode
    {
    private:
        std::vector<SyntaxNode*> _nodes;
        bool _to_return;
    public:
        SequenceExpressionSyntax(std::vector<SyntaxNode*>& nodes, bool to_return=false);
        ~SequenceExpressionSyntax();
        SyntaxKind kind() const;    

        int get_nodes_size() const;
        SyntaxNode* get_node(int i);
        std::vector<SyntaxNode*> get_nodes();
        bool get_to_return();
    };

    // Refer to while-syntax.cpp.
    class WhileExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _condition;
        SyntaxNode* _body;
    public:
        WhileExpressionSyntax(SyntaxNode* condition, SyntaxNode* body);
        ~WhileExpressionSyntax();

        SyntaxKind kind() const;

        SyntaxNode* get_condition();
        SyntaxNode* get_body();
    };

    // Refer to for-syntax.cpp.
    class ForExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode *_init, *_condition, *_update, *_body;
    public:
        ForExpressionSyntax(SyntaxNode* init, SyntaxNode* condition, SyntaxNode* update, SyntaxNode* body);\
        ~ForExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxNode* get_init();     
        SyntaxNode* get_condition();      
        SyntaxNode* get_update();      
        SyntaxNode* get_body();            
    };

    // Refer to var-declare-syntax.cpp.
    class VarDeclareExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _var_keyword;
        SyntaxToken _identifier;
    public:
        VarDeclareExpressionSyntax(SyntaxToken var_keyword, SyntaxToken identifier);
        ~VarDeclareExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxToken* get_var_keyword();
        SyntaxToken* get_identifier();
    };

    // Refer to var-assign-syntax.cpp.
    class VarAssignExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
        SyntaxNode* _value;
    public:
        VarAssignExpressionSyntax(SyntaxToken identifier, SyntaxNode* value);
        ~VarAssignExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxToken* get_identifier();
        SyntaxNode* get_value();
    };

    // Refer to var-access-syntax.cpp.
    class VarAccessExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
    public:
        VarAccessExpressionSyntax(SyntaxToken identifier);
        ~VarAccessExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxToken* get_identifier();
    };

    // Refer to if-syntax.cpp.
    class IfExpressionSyntax final : public SyntaxNode
    {
    private:
        std::vector<SyntaxNode*> _conditions;
        std::vector<SyntaxNode*> _bodies;
        SyntaxNode* _else_body;
    public:
        IfExpressionSyntax(std::vector<SyntaxNode*>& conditions,std::vector<SyntaxNode*>& bodies, SyntaxNode* else_body);
        ~IfExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxNode* get_else_body();
        
        int get_size();
        SyntaxNode* get_condition(int i);
        SyntaxNode* get_body(int i);

        std::vector<SyntaxNode*> get_conditions();
        std::vector<SyntaxNode*> get_bodies();
    };

    // Refer to func-define-syntax.cpp.
    class FuncDefineExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
        std::vector<SyntaxToken> _arg_names;
        SyntaxNode* _body;
    public:
        FuncDefineExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxToken>& arg_names, SyntaxNode* body);
        ~FuncDefineExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxToken* get_identifier();
        int get_arg_size();
        SyntaxToken* get_arg_name(int i);
        std::vector<SyntaxToken> get_arg_names();
        SyntaxNode* get_body();
    };

    // Refer to func-call-syntax.cpp.
    class FuncCallExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
        std::vector<SyntaxNode*> _args;
    public:
        FuncCallExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxNode*>& args);
        ~FuncCallExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxToken* get_identifier();
        int get_arg_size();
        SyntaxNode* get_arg(int i);
        std::vector<SyntaxNode*> get_args();
    }; 

    // Refer to index-syntax.cpp.
    class IndexExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _to_access;
        SyntaxNode* _indexer;
    public:
        IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer);
        ~IndexExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxNode* get_to_access();
        SyntaxNode* get_indexer();
    };

    // Refer to return-syntax.cpp.
    class ReturnExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _to_return;
    public:
        ReturnExpressionSyntax(SyntaxNode* to_return);
        ~ReturnExpressionSyntax();

        SyntaxKind kind() const;
        SyntaxNode* get_to_return();
    };

    // Refer to continue-syntax.cpp.
    class ContinueExpressionSyntax final : public SyntaxNode
    {
    public:
        ContinueExpressionSyntax();
        ~ContinueExpressionSyntax();

        SyntaxKind kind() const;
    };

    // Refer to break-syntax.cpp.
    class BreakExpressionSyntax final : public SyntaxNode
    {
    public:
        BreakExpressionSyntax();
        ~BreakExpressionSyntax();

        SyntaxKind kind() const;
    };

    // Refer to none-syntax.cpp.
    class NoneExpressionSyntax final : public SyntaxNode
    {
    public:
        NoneExpressionSyntax();
        ~NoneExpressionSyntax();

        SyntaxKind kind() const;
    };

    // Refer to parser.cpp.
    class Parser final
    {
    private:
        std::vector<SyntaxToken> _tokens;
        int _position;
        bool _show_return;

        SyntaxToken peek(int offset) const;
        SyntaxToken current() const;
        SyntaxToken look_ahead() const;
        SyntaxToken next_token();
        SyntaxToken match_token(SyntaxKind kind);

        SyntaxNode* parse_atom();
        SyntaxNode* parse_molecule();
        SyntaxNode* parse_expression(int precedence = 0);
        SyntaxNode* parse_statement();
        SyntaxNode* parse_program(bool sub_program=false);
    public:
        Parser(std::string& text, bool show_return);
        SyntaxNode* parse();
    };
}