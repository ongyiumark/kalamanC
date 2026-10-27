#pragma once

#include "../syntax.h"
#include <memory>

namespace Syntax
{
    // Refer to literal-syntax.cpp.
    class LiteralExpressionSyntax final : public SyntaxNode
    {
    private:
        Objects::Object* _value;
    public:
        LiteralExpressionSyntax(Objects::Object* value, Diagnostics::Position pos);
        ~LiteralExpressionSyntax() override;

        SyntaxKind kind() const override;
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
        ~UnaryExpressionSyntax() override;

        SyntaxKind kind() const override;
        
        SyntaxToken* get_op_token();
        SyntaxNode* get_operand();
    };

    // Refer to binary-syntax.cpp.
    class BinaryExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _left;
        SyntaxToken _op_token;
        SyntaxNode* _right;
    public:
        BinaryExpressionSyntax(SyntaxNode* left, SyntaxToken op_token, SyntaxNode* right, Diagnostics::Position pos);
        ~BinaryExpressionSyntax() override;

        SyntaxKind kind() const override;
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
        bool _is_block;
    public:
        SequenceExpressionSyntax(std::vector<SyntaxNode*>& nodes, Diagnostics::Position pos, bool to_return=false, bool is_block=false);
        ~SequenceExpressionSyntax() override;
        SyntaxKind kind() const override;    

        int get_nodes_size() const;
        SyntaxNode* get_node(int i);
        std::vector<SyntaxNode*> get_nodes();
        bool get_to_return();
        bool is_block() const;
    };

    // Refer to while-syntax.cpp.
    class WhileExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _condition;
        SyntaxNode* _body;
    public:
        WhileExpressionSyntax(SyntaxNode* condition, SyntaxNode* body, Diagnostics::Position pos);
        ~WhileExpressionSyntax() override;

        SyntaxKind kind() const override;

        SyntaxNode* get_condition();
        SyntaxNode* get_body();
    };

    // Refer to for-syntax.cpp.
    class ForExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode *_init, *_condition, *_update, *_body;
        bool _is_foreach;
    public:
        ForExpressionSyntax(SyntaxNode* init, SyntaxNode* condition, SyntaxNode* update, SyntaxNode* body, Diagnostics::Position pos, bool is_foreach = false);
        ~ForExpressionSyntax() override;

        SyntaxKind kind() const override;
        SyntaxNode* get_init();     
        SyntaxNode* get_condition();      
        SyntaxNode* get_update();      
        SyntaxNode* get_body();
        bool is_foreach() const;
    };

    // Refer to var-declare-syntax.cpp.
    class VarDeclareExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _var_keyword;
        SyntaxToken _identifier;
    public:
        VarDeclareExpressionSyntax(SyntaxToken var_keyword, SyntaxToken identifier, Diagnostics::Position pos);
        ~VarDeclareExpressionSyntax() override;

        SyntaxKind kind() const override;
        SyntaxToken* get_var_keyword();
        SyntaxToken* get_identifier();
    };

    // Refer to var-assign-syntax.cpp.
    class VarAssignExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
        SyntaxToken _op_token;
        SyntaxNode* _target;
        SyntaxNode* _value;
    public:
        VarAssignExpressionSyntax(SyntaxToken identifier, SyntaxNode* value, Diagnostics::Position pos);
        VarAssignExpressionSyntax(SyntaxToken identifier, SyntaxToken op_token, SyntaxNode* value, Diagnostics::Position pos);
        VarAssignExpressionSyntax(SyntaxNode* target, SyntaxToken op_token, SyntaxNode* value, Diagnostics::Position pos);
        ~VarAssignExpressionSyntax() override;

        SyntaxKind kind() const override;
        SyntaxToken* get_identifier();
        SyntaxToken* get_op_token();
        SyntaxNode* get_target();
        SyntaxNode* get_value();
    };

    // Refer to var-access-syntax.cpp.
    class VarAccessExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
    public:
        VarAccessExpressionSyntax(SyntaxToken identifier, Diagnostics::Position pos);
        ~VarAccessExpressionSyntax() override;

        SyntaxKind kind() const override;
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
        IfExpressionSyntax(std::vector<SyntaxNode*>& conditions, std::vector<SyntaxNode*>& bodies, SyntaxNode* else_body, 
            Diagnostics::Position pos);
        ~IfExpressionSyntax() override;

        SyntaxKind kind() const override;
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
        std::vector<Objects::Type> _arg_types;
        std::shared_ptr<SyntaxNode> _body;
    public:
        FuncDefineExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxToken>& arg_names, 
            const std::vector<Objects::Type>& arg_types, SyntaxNode* body, 
            Diagnostics::Position pos);
        ~FuncDefineExpressionSyntax() override;

        SyntaxKind kind() const override;
        SyntaxToken* get_identifier();
        int get_arg_size();
        SyntaxToken* get_arg_name(int i);
        std::vector<SyntaxToken> get_arg_names();
        const std::vector<Objects::Type>& get_arg_types() const;
        SyntaxNode* get_body();
        std::shared_ptr<SyntaxNode> get_body_shared();
    };

    // Refer to func-call-syntax.cpp.
    class FuncCallExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxToken _identifier;
        std::vector<SyntaxNode*> _args;
    public:
        FuncCallExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxNode*>& args, Diagnostics::Position pos);
        ~FuncCallExpressionSyntax() override;

        SyntaxKind kind() const override;
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
        SyntaxNode* _slice_end;
        bool _is_slice;
    public:
        IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* indexer, Diagnostics::Position pos);
        IndexExpressionSyntax(SyntaxNode* to_access, SyntaxNode* start, SyntaxNode* end, bool is_slice, Diagnostics::Position pos);
        ~IndexExpressionSyntax() override;

        SyntaxKind kind() const override;
        SyntaxNode* get_to_access();
        SyntaxNode* get_indexer();
        SyntaxNode* get_slice_end();
        bool is_slice() const;
    };

    // Refer to return-syntax.cpp.
    class ReturnExpressionSyntax final : public SyntaxNode
    {
    private:
        SyntaxNode* _to_return;
    public:
        ReturnExpressionSyntax(SyntaxNode* to_return, Diagnostics::Position pos = Diagnostics::Position());
        ~ReturnExpressionSyntax() override;

        SyntaxKind kind() const override;
        SyntaxNode* get_to_return();
    };

    // Refer to continue-syntax.cpp.
    class ContinueExpressionSyntax final : public SyntaxNode
    {
    public:
        ContinueExpressionSyntax(Diagnostics::Position pos = Diagnostics::Position());
        ~ContinueExpressionSyntax() override;

        SyntaxKind kind() const override;
    };

    // Refer to break-syntax.cpp.
    class BreakExpressionSyntax final : public SyntaxNode
    {
    public:
        BreakExpressionSyntax(Diagnostics::Position pos = Diagnostics::Position());
        ~BreakExpressionSyntax() override;

        SyntaxKind kind() const override;
    };

    // Refer to none-syntax.cpp.
    class NoneExpressionSyntax final : public SyntaxNode
    {
    public:
        NoneExpressionSyntax(Diagnostics::Position pos = Diagnostics::Position());
        ~NoneExpressionSyntax() override;

        SyntaxKind kind() const override;
    };

    // Refer to parser.cpp.
    class Parser final
    {
    private:
        std::vector<SyntaxToken> _tokens;
        int _position;
        int _loop_depth;
        int _func_depth;

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
        Parser(std::string& text, bool show_return = false);
        SyntaxNode* parse();
    };
}
