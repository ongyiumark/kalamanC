#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#if defined(__APPLE__) || defined(__unix__)
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define KALAMANC_HAS_READLINE 1
#else
#define KALAMANC_HAS_READLINE 0
#endif

#include "Evaluators/initialize.h"

using Evaluators::initialize;
using Evaluators::run;

static void print_help(const char* prog_name)
{
    std::cout << "Usage: " << prog_name << " [options] [script.kal]\n\n"
              << "Options:\n"
              << "  -e, -c <code>       Execute inline KalamanC code\n"
              << "  -t, --tree          Display the syntax parse tree\n"
              << "  --lang=<en|fil>     Set output language for booleans (english or filipino)\n"
              << "  --recursion-limit=<N> Set maximum recursion call depth (default: 1000)\n"
              << "  -h, --help          Show this help message\n"
              << "  -v, --version       Show version information\n\n"
              << "Run without a script file to launch the interactive shell (REPL)." << std::endl;
}

static void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}

// Checks if the accumulated REPL buffer has unclosed braces/brackets/parens or block comments.
static bool needs_more_input(const std::string& buffer)
{
    int curly = 0, paren = 0, square = 0;
    bool in_string = false;
    char quote_char = '\0';
    bool in_block_comment = false;

    for (size_t i = 0; i < buffer.size(); i++)
    {
        char c = buffer[i];
        char next = (i + 1 < buffer.size()) ? buffer[i + 1] : '\0';

        if (in_block_comment)
        {
            if (c == '*' && next == '/')
            {
                in_block_comment = false;
                i++;
            }
            continue;
        }

        if (in_string)
        {
            if (c == '\\' && next != '\0')
            {
                i++;
                continue;
            }
            if (c == quote_char)
                in_string = false;
            continue;
        }

        if (c == '/' && next == '/')
        {
            while (i < buffer.size() && buffer[i] != '\n')
                i++;
            continue;
        }

        if (c == '/' && next == '*')
        {
            in_block_comment = true;
            i++;
            continue;
        }

        if (c == '"' || c == '\'')
        {
            in_string = true;
            quote_char = c;
            continue;
        }

        if (c == '{') curly++;
        else if (c == '}') curly--;
        else if (c == '(') paren++;
        else if (c == ')') paren--;
        else if (c == '[') square++;
        else if (c == ']') square--;
    }

    return in_block_comment || curly > 0 || paren > 0 || square > 0;
}

static std::string trim_copy(const std::string& s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static bool read_repl_line(const char* prompt, std::string& out_line)
{
#if KALAMANC_HAS_READLINE
    if (isatty(STDIN_FILENO))
    {
        char* raw = readline(prompt);
        if (!raw)
        {
            std::cout << std::endl;
            return false;
        }
        out_line = raw;
        if (!trim_copy(out_line).empty())
            add_history(raw);
        free(raw);
        return true;
    }
#endif
    std::cout << prompt << std::flush;
    if (!std::getline(std::cin, out_line))
    {
        std::cout << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char ** argv)
{
    initialize();

    bool show_tree = false;
    bool show_return = true;
    std::string inline_code;
    bool has_inline_code = false;
    std::string filename;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help")
        {
            print_help(argv[0]);
            return 0;
        }
        else if (arg == "-v" || arg == "--version")
        {
            std::cout << "KalamanC 2.0 (Modernized Edition)" << std::endl;
            return 0;
        }
        else if (arg == "-t" || arg == "--tree")
        {
            show_tree = true;
        }
        else if (arg == "-e" || arg == "-c")
        {
            if (i + 1 >= argc)
            {
                std::cerr << "ERROR: option '" << arg << "' requires a code argument" << std::endl;
                return 1;
            }
            inline_code = argv[++i];
            has_inline_code = true;
        }
        else if (arg.rfind("--lang=", 0) == 0)
        {
            std::string lang = arg.substr(7);
            if (lang == "fil" || lang == "filipino" || lang == "tagalog")
            {
                Objects::Boolean::use_filipino = true;
                Objects::Boolean::lang_locked = true;
            }
            else if (lang == "en" || lang == "english")
            {
                Objects::Boolean::use_filipino = false;
                Objects::Boolean::lang_locked = true;
            }
            else if (lang == "auto")
            {
                Objects::Boolean::lang_locked = false;
            }
            else
            {
                std::cerr << "ERROR: unknown language '" << lang << "' (use 'english', 'filipino', or 'auto')" << std::endl;
                return 1;
            }
        }
        else if (arg.rfind("--recursion-limit=", 0) == 0)
        {
            try
            {
                int limit = std::stoi(arg.substr(18));
                if (limit < 1)
                {
                    std::cerr << "ERROR: recursion limit must be at least 1" << std::endl;
                    return 1;
                }
                Evaluators::Evaluator::max_recursion_depth = limit;
            }
            catch (...)
            {
                std::cerr << "ERROR: invalid recursion limit value in '" << arg << "'" << std::endl;
                return 1;
            }
        }
        else if (!arg.empty() && arg[0] == '-')
        {
            std::cerr << "ERROR: unknown option '" << arg << "'. Use --help for usage." << std::endl;
            return 1;
        }
        else
        {
            filename = arg;
        }
    }

    if (has_inline_code)
    {
        bool ok = run(inline_code, show_tree, false, false);
        return ok ? 0 : 1;
    }

    if (!filename.empty())
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "ERROR: could not open script file '" << filename << "'" << std::endl;
            return 1;
        }
        std::ostringstream ss;
        ss << file.rdbuf();
        std::string script = ss.str();
        file.close();

        bool ok = run(script, show_tree, false, false);
        return ok ? 0 : 1;
    }

    // Interactive REPL mode
    std::cout << "KalamanC 2.0 (Interactive Shell)" << std::endl;
    std::cout << "Supports both English and Filipino keywords! Type \"#help\" for commands or \"#exit\" to quit.\n" << std::endl;

    while (true)
    {
        std::string line;
        if (!read_repl_line(">>> ", line))
            break;

        std::string trimmed = trim_copy(line);
        if (trimmed.empty())
            continue;

        if (trimmed == "#exit" || trimmed == "#quit")
            break;

        if (trimmed == "#help")
        {
            std::cout << "Shell commands begin with '#':\n" << std::endl;
            std::cout << "  #showtree          - toggles displaying the parse tree (default: false)" << std::endl;
            std::cout << "  #showreturn        - toggles displaying return values of expressions (default: true)" << std::endl;
            std::cout << "  #lang [en|fil|auto]- shows or sets boolean output language (true/false vs totoo/mali)" << std::endl;
            std::cout << "  #vars              - lists all currently defined variables and functions" << std::endl;
            std::cout << "  #reset             - clears all variables and resets the session" << std::endl;
            std::cout << "  #cls / #clear      - clears the terminal screen" << std::endl;
            std::cout << "  #exit / #quit      - exits the interactive shell\n" << std::endl;
            continue;
        }

        if (trimmed == "#showtree")
        {
            show_tree = !show_tree;
            std::cout << (show_tree ? "Showing parse tree..." : "Not showing parse tree...") << "\n" << std::endl;
            continue;
        }

        if (trimmed == "#showreturn")
        {
            show_return = !show_return;
            std::cout << (show_return ? "Showing return values..." : "Not showing return values...") << "\n" << std::endl;
            continue;
        }

        if (trimmed == "#cls" || trimmed == "#clear")
        {
            clear_screen();
            continue;
        }

        if (trimmed == "#vars" || trimmed == "#symbols")
        {
            Evaluators::print_symbols();
            continue;
        }

        if (trimmed == "#reset")
        {
            Evaluators::reset();
            std::cout << "Session reset.\n" << std::endl;
            continue;
        }

        if (trimmed.rfind("#lang", 0) == 0)
        {
            std::string arg = trim_copy(trimmed.substr(5));
            if (arg.empty())
            {
                std::cout << "Current output language: "
                          << (Objects::Boolean::use_filipino ? "Filipino (totoo/mali)" : "English (true/false)")
                          << (Objects::Boolean::lang_locked ? " [locked]" : " [auto]") << "\n" << std::endl;
            }
            else if (arg == "fil" || arg == "filipino")
            {
                Objects::Boolean::use_filipino = true;
                Objects::Boolean::lang_locked = true;
                std::cout << "Output language set to Filipino (totoo/mali).\n" << std::endl;
            }
            else if (arg == "en" || arg == "english")
            {
                Objects::Boolean::use_filipino = false;
                Objects::Boolean::lang_locked = true;
                std::cout << "Output language set to English (true/false).\n" << std::endl;
            }
            else if (arg == "auto")
            {
                Objects::Boolean::lang_locked = false;
                std::cout << "Output language set to auto-detect.\n" << std::endl;
            }
            else
            {
                std::cout << "Usage: #lang [english|filipino|auto]\n" << std::endl;
            }
            continue;
        }

        // Accumulate multi-line blocks when braces/parens/brackets are open
        std::string buffer = line;
        while (needs_more_input(buffer))
        {
            std::string next_line;
            if (!read_repl_line("... ", next_line))
                break;
            buffer += "\n" + next_line;
        }

        // Convenience in REPL: if a single-line expression/statement omits the trailing semicolon, append one
        std::string final_trimmed = trim_copy(buffer);
        if (!final_trimmed.empty() && final_trimmed.back() != ';' && final_trimmed.back() != '}')
        {
            buffer += ";";
        }

        run(buffer, show_tree, show_return, true);
    }

    return 0;
}
