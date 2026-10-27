#include "syntax.h"
#include "../constants.h"
#include <unordered_map>
#include <unordered_set>

using namespace Syntax;

// Fast hash-map lookup supporting both English and Filipino keywords and built-in functions.
SyntaxKind SyntaxFacts::get_keyword_kind(const std::string& text)
{
    static const std::unordered_map<std::string, SyntaxKind> keyword_map = {
        // English Keywords
        {KT_TRUE,       SyntaxKind::TrueKeyword},
        {KT_FALSE,      SyntaxKind::FalseKeyword},
        {KT_BOOL,       SyntaxKind::BooleanKeyword},
        {KT_BOOL_SHORT, SyntaxKind::BooleanKeyword},
        {KT_INTEGER,    SyntaxKind::IntegerKeyword},
        {KT_DOUBLE,     SyntaxKind::DoubleKeyword},
        {KT_LIST,       SyntaxKind::ListKeyword},
        {KT_STRING,     SyntaxKind::StringKeyword},
        {KT_FUNCTION,   SyntaxKind::FunctionKeyword},
        {KT_FUNC_SHORT, SyntaxKind::FunctionKeyword},
        {KT_DEFINE,     SyntaxKind::DefineFunctionKeyword},
        {KT_DEF_SHORT,  SyntaxKind::DefineFunctionKeyword},
        {KT_IF,         SyntaxKind::IfKeyword},
        {KT_ELIF,       SyntaxKind::ElifKeyword},
        {KT_ELSE,       SyntaxKind::ElseKeyword},
        {KT_WHILE,      SyntaxKind::WhileKeyword},
        {KT_FOR,        SyntaxKind::ForKeyword},
        {KT_IN,         SyntaxKind::InKeyword},
        {KT_AND,        SyntaxKind::AndKeyword},
        {KT_OR,         SyntaxKind::OrKeyword},
        {KT_XOR,        SyntaxKind::XorKeyword},
        {KT_NOT,        SyntaxKind::NotKeyword},
        {KT_RETURN,     SyntaxKind::ReturnKeyword},
        {KT_BREAK,      SyntaxKind::BreakKeyword},
        {KT_CONTINUE,   SyntaxKind::ContinueKeyword},

        // Filipino Keywords
        {KT_TRUE_FIL,     SyntaxKind::TrueKeyword},
        {KT_FALSE_FIL,    SyntaxKind::FalseKeyword},
        {KT_BOOL_FIL,     SyntaxKind::BooleanKeyword},
        {KT_INTEGER_FIL,  SyntaxKind::IntegerKeyword},
        {KT_DOUBLE_FIL,   SyntaxKind::DoubleKeyword},
        {KT_LIST_FIL,     SyntaxKind::ListKeyword},
        {KT_STRING_FIL,   SyntaxKind::StringKeyword},
        {KT_FUNCTION_FIL, SyntaxKind::FunctionKeyword},
        {KT_DEFINE_FIL,   SyntaxKind::DefineFunctionKeyword},
        {KT_IF_FIL,       SyntaxKind::IfKeyword},
        {KT_ELIF_FIL,     SyntaxKind::ElifKeyword},
        {KT_ELSE_FIL,     SyntaxKind::ElseKeyword},
        {KT_WHILE_FIL,    SyntaxKind::WhileKeyword},
        {KT_FOR_FIL,      SyntaxKind::ForKeyword},
        {KT_IN_FIL,       SyntaxKind::InKeyword},
        {KT_AND_FIL,      SyntaxKind::AndKeyword},
        {KT_OR_FIL,       SyntaxKind::OrKeyword},
        {KT_XOR_FIL,      SyntaxKind::XorKeyword},
        {KT_NOT_FIL,      SyntaxKind::NotKeyword},
        {KT_RETURN_FIL,   SyntaxKind::ReturnKeyword},
        {KT_BREAK_FIL,    SyntaxKind::BreakKeyword},
        {KT_CONTINUE_FIL, SyntaxKind::ContinueKeyword},

        // English Built-in Functions
        {BI_PRINT,      SyntaxKind::PrintFunction},
        {BI_INPUT,      SyntaxKind::InputFunction},
        {BI_SPLIT,      SyntaxKind::SplitFunction},
        {BI_SIZE,       SyntaxKind::SizeFunction},
        {BI_LEN,        SyntaxKind::SizeFunction},
        {BI_TYPE,       SyntaxKind::TypeFunction},
        {BI_TO_BOOL,    SyntaxKind::ToBoolFunction},
        {BI_TO_INT,     SyntaxKind::ToIntFunction},
        {BI_TO_DOUBLE,  SyntaxKind::ToDoubleFunction},
        {BI_TO_STRING,  SyntaxKind::ToStringFunction},
        {BI_SET_INDEX,  SyntaxKind::SetIndexFunction},
        {BI_PUSH,       SyntaxKind::PushFunction},
        {BI_APPEND,     SyntaxKind::PushFunction},
        {BI_POP,        SyntaxKind::PopFunction},
        {BI_RANGE,      SyntaxKind::RangeFunction},
        {BI_SORT,       SyntaxKind::SortFunction},
        {BI_REVERSE,    SyntaxKind::ReverseFunction},
        {BI_JOIN,       SyntaxKind::JoinFunction},
        {BI_ABS,        SyntaxKind::AbsFunction},
        {BI_MIN,        SyntaxKind::MinFunction},
        {BI_MAX,        SyntaxKind::MaxFunction},
        {BI_SQRT,       SyntaxKind::SqrtFunction},
        {BI_POW_MOD,    SyntaxKind::PowModFunction},
        {BI_SET_MOD,    SyntaxKind::SetModFunction},
        {BI_SET_RECURSION_LIMIT, SyntaxKind::SetRecursionLimitFunction},
        {BI_GET_RECURSION_LIMIT, SyntaxKind::GetRecursionLimitFunction},
        {BI_READ_FILE,  SyntaxKind::ReadFileFunction},
        {BI_WRITE_FILE, SyntaxKind::WriteFileFunction},
        {BI_CLOCK,      SyntaxKind::ClockFunction},
        {BI_EXIT,       SyntaxKind::ExitFunction},

        // Filipino Built-in Functions
        {BI_PRINT_FIL,      SyntaxKind::PrintFunction},
        {BI_INPUT_FIL,      SyntaxKind::InputFunction},
        {BI_SPLIT_FIL,      SyntaxKind::SplitFunction},
        {BI_SIZE_FIL,       SyntaxKind::SizeFunction},
        {BI_TYPE_FIL,       SyntaxKind::TypeFunction},
        {BI_TO_BOOL_FIL,    SyntaxKind::ToBoolFunction},
        {BI_TO_INT_FIL,     SyntaxKind::ToIntFunction},
        {BI_TO_DOUBLE_FIL,  SyntaxKind::ToDoubleFunction},
        {BI_TO_STRING_FIL,  SyntaxKind::ToStringFunction},
        {BI_SET_INDEX_FIL,  SyntaxKind::SetIndexFunction},
        {BI_PUSH_FIL,       SyntaxKind::PushFunction},
        {BI_POP_FIL,        SyntaxKind::PopFunction},
        {BI_RANGE_FIL,      SyntaxKind::RangeFunction},
        {BI_SORT_FIL,       SyntaxKind::SortFunction},
        {BI_REVERSE_FIL,    SyntaxKind::ReverseFunction},
        {BI_JOIN_FIL,       SyntaxKind::JoinFunction},
        {BI_ABS_FIL,        SyntaxKind::AbsFunction},
        {BI_MIN_FIL,        SyntaxKind::MinFunction},
        {BI_MAX_FIL,        SyntaxKind::MaxFunction},
        {BI_SQRT_FIL,       SyntaxKind::SqrtFunction},
        {BI_POW_MOD_FIL,    SyntaxKind::PowModFunction},
        {BI_SET_MOD_FIL,    SyntaxKind::SetModFunction},
        {BI_SET_RECURSION_LIMIT_FIL, SyntaxKind::SetRecursionLimitFunction},
        {BI_GET_RECURSION_LIMIT_FIL, SyntaxKind::GetRecursionLimitFunction},
        {BI_READ_FILE_FIL,  SyntaxKind::ReadFileFunction},
        {BI_WRITE_FILE_FIL, SyntaxKind::WriteFileFunction},
        {BI_CLOCK_FIL,      SyntaxKind::ClockFunction},
        {BI_EXIT_FIL,       SyntaxKind::ExitFunction},
    };

    auto it = keyword_map.find(text);
    if (it != keyword_map.end())
        return it->second;
    return SyntaxKind::IdentifierToken;
}

void SyntaxFacts::detect_language(const std::string& text)
{
    if (Objects::Boolean::lang_locked) return;

    static const std::unordered_set<std::string> filipino_markers = {
        KT_TRUE_FIL, KT_FALSE_FIL, KT_INTEGER_FIL, KT_DOUBLE_FIL, KT_LIST_FIL,
        KT_STRING_FIL, KT_DEFINE_FIL, KT_IF_FIL, KT_ELIF_FIL, KT_ELSE_FIL,
        KT_WHILE_FIL, KT_FOR_FIL, KT_IN_FIL, KT_AND_FIL, KT_OR_FIL, KT_XOR_FIL,
        KT_NOT_FIL, KT_RETURN_FIL, KT_BREAK_FIL, KT_CONTINUE_FIL,
        BI_PRINT_FIL, BI_INPUT_FIL, BI_SPLIT_FIL, BI_SIZE_FIL, BI_TYPE_FIL,
        BI_TO_BOOL_FIL, BI_TO_INT_FIL, BI_TO_DOUBLE_FIL, BI_TO_STRING_FIL,
        BI_SET_INDEX_FIL, BI_PUSH_FIL, BI_POP_FIL, BI_RANGE_FIL, BI_SORT_FIL,
        BI_REVERSE_FIL, BI_JOIN_FIL, BI_ABS_FIL, BI_MIN_FIL, BI_MAX_FIL,
        BI_SQRT_FIL, BI_SET_MOD_FIL, BI_SET_RECURSION_LIMIT_FIL,
        BI_GET_RECURSION_LIMIT_FIL, BI_READ_FILE_FIL, BI_WRITE_FILE_FIL,
        BI_CLOCK_FIL, BI_EXIT_FIL, VAR_ARGS_FIL
    };

    static const std::unordered_set<std::string> english_markers = {
        KT_TRUE, KT_FALSE, KT_INTEGER, KT_DOUBLE, KT_LIST, KT_STRING,
        KT_DEFINE, KT_DEF_SHORT, KT_IF, KT_ELIF, KT_ELSE, KT_WHILE, KT_FOR,
        KT_RETURN, KT_BREAK, KT_CONTINUE, BI_PRINT, BI_INPUT, BI_SPLIT,
        BI_SIZE, BI_JOIN, BI_READ_FILE, BI_WRITE_FILE, BI_CLOCK, BI_EXIT,
        VAR_ARGS
    };

    if (filipino_markers.count(text))
    {
        Objects::Boolean::use_filipino = true;
        Objects::Boolean::lang_locked = true;
    }
    else if (english_markers.count(text))
    {
        Objects::Boolean::use_filipino = false;
        Objects::Boolean::lang_locked = true;
    }
}

bool SyntaxFacts::is_builtin_function(SyntaxKind kind)
{
    switch (kind)
    {
        case SyntaxKind::PrintFunction:
        case SyntaxKind::InputFunction:
        case SyntaxKind::SplitFunction:
        case SyntaxKind::SizeFunction:
        case SyntaxKind::TypeFunction:
        case SyntaxKind::ToBoolFunction:
        case SyntaxKind::ToIntFunction:
        case SyntaxKind::ToDoubleFunction:
        case SyntaxKind::ToStringFunction:
        case SyntaxKind::SetIndexFunction:
        case SyntaxKind::PushFunction:
        case SyntaxKind::PopFunction:
        case SyntaxKind::RangeFunction:
        case SyntaxKind::SortFunction:
        case SyntaxKind::ReverseFunction:
        case SyntaxKind::JoinFunction:
        case SyntaxKind::AbsFunction:
        case SyntaxKind::MinFunction:
        case SyntaxKind::MaxFunction:
        case SyntaxKind::SqrtFunction:
        case SyntaxKind::PowModFunction:
        case SyntaxKind::SetModFunction:
        case SyntaxKind::SetRecursionLimitFunction:
        case SyntaxKind::GetRecursionLimitFunction:
        case SyntaxKind::ReadFileFunction:
        case SyntaxKind::WriteFileFunction:
        case SyntaxKind::ClockFunction:
        case SyntaxKind::ExitFunction:
            return true;
        default:
            return false;
    }
}

bool SyntaxFacts::is_type_keyword(SyntaxKind kind)
{
    switch (kind)
    {
        case SyntaxKind::IntegerKeyword:
        case SyntaxKind::DoubleKeyword:
        case SyntaxKind::BooleanKeyword:
        case SyntaxKind::ListKeyword:
        case SyntaxKind::FunctionKeyword:
        case SyntaxKind::StringKeyword:
            return true;
        default:
            return false;
    }
}

bool SyntaxFacts::is_assignment_operator(SyntaxKind kind)
{
    switch (kind)
    {
        case SyntaxKind::EqualsToken:
        case SyntaxKind::PlusEqualsToken:
        case SyntaxKind::MinusEqualsToken:
        case SyntaxKind::StarEqualsToken:
        case SyntaxKind::SlashEqualsToken:
        case SyntaxKind::ModuloEqualsToken:
        case SyntaxKind::PowerEqualsToken:
            return true;
        default:
            return false;
    }
}

Objects::Type SyntaxFacts::get_keyword_type(SyntaxKind kind)
{
    switch (kind)
    {
        case SyntaxKind::IntegerKeyword:
            return Objects::Type::INTEGER;
        case SyntaxKind::DoubleKeyword:
            return Objects::Type::DOUBLE;
        case SyntaxKind::BooleanKeyword:
            return Objects::Type::BOOLEAN;
        case SyntaxKind::ListKeyword:
            return Objects::Type::LIST;
        case SyntaxKind::FunctionKeyword:
            return Objects::Type::FUNCTION;
        case SyntaxKind::StringKeyword:
            return Objects::Type::STRING;
        default:
            return Objects::Type::NONE;
    }
}

// Operator precedences.
int SyntaxFacts::get_unary_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
        case SyntaxKind::BangToken:
        case SyntaxKind::NotKeyword:
            return 6;
        default:
            return 0;
    }
}

int SyntaxFacts::get_binary_precedence(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PowerToken:
            return 7;
        case SyntaxKind::StarToken:
        case SyntaxKind::SlashToken:
        case SyntaxKind::ModuloToken:
            return 5;
        case SyntaxKind::PlusToken:
        case SyntaxKind::MinusToken:
            return 4;
        case SyntaxKind::DEqualsToken:
        case SyntaxKind::BangEqualsToken:
        case SyntaxKind::LessEqualsToken:
        case SyntaxKind::LessThanToken:
        case SyntaxKind::GreaterEqualsToken:
        case SyntaxKind::GreaterThanToken:
            return 3;
        case SyntaxKind::DAmpersandToken:
        case SyntaxKind::AndKeyword:
            return 2;
        case SyntaxKind::DPipeToken:
        case SyntaxKind::OrKeyword:
        case SyntaxKind::XorKeyword:
            return 1;
        default:
            return 0;
    }
}
