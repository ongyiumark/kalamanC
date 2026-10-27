#!/usr/bin/env bash
set -euo pipefail

BIN="./kalman"
PASS=0
FAIL=0

assert_eq() {
    local name="$1"
    local actual="$2"
    local expected="$3"
    if [ "$actual" = "$expected" ]; then
        echo "  [PASS] $name"
        PASS=$((PASS + 1))
    else
        echo "  [FAIL] $name"
        echo "    Expected: $(printf '%q' "$expected")"
        echo "    Actual:   $(printf '%q' "$actual")"
        FAIL=$((FAIL + 1))
    fi
}

assert_contains() {
    local name="$1"
    local actual="$2"
    local substr="$3"
    if [[ "$actual" == *"$substr"* ]]; then
        echo "  [PASS] $name"
        PASS=$((PASS + 1))
    else
        echo "  [FAIL] $name"
        echo "    Expected to contain: $substr"
        echo "    Actual: $actual"
        FAIL=$((FAIL + 1))
    fi
}

echo "Running KalamanC 2.0 Test Suite..."

# 1. Arithmetic, Modulo Precedence, and Right-Associative Power
assert_eq "Arithmetic & power precedence" \
    "$($BIN -e 'print((2+1)^2*2, 1 + 10 % 3, 2 ^ 3 ^ 2);')" \
    "18 2 512"

# 2. Logical Operators (including fixed XOR and short-circuit && / ||)
assert_eq "Logical operators and XOR" \
    "$($BIN -e 'print(true xor false, true xor true, false at true, totoo xo mali);' --lang=en)" \
    "true false false true"

assert_eq "Short-circuit AND and OR" \
    "$($BIN -e 'list a = [10, 20]; int i = 5; print(i < size(a) && a[i] == 10, i == 5 || a[i] == 10);')" \
    "false true"

# 3. Bilingual Keywords (Filipino & English auto-detection)
assert_eq "Filipino script execution & boolean output" \
    "$($BIN -e 'bilang x = 5; kung (x > 3) lathala(totoo); kundi lathala(mali);')" \
    "totoo"

assert_eq "English script execution & boolean output" \
    "$($BIN -e 'int x = 5; if (x > 3) print(true); else print(false);')" \
    "true"

# 4. Variable Scoping & Shadowing / Redefinition Prevention
REDEF_SHADOW_ERR="$($BIN -e '
    int x = 100;
    def rec(int n) {
        int x = n * 10;
        return x;
    }
    rec(3);
' 2>&1 || true)"
assert_contains "Function variable shadowing outer variable error" "$REDEF_SHADOW_ERR" "redefinition of variable 'x'"

REDEF_SAME_SCOPE_ERR="$($BIN -e 'int a = 1; int a = 2;' 2>&1 || true)"
assert_contains "Same scope redefinition error" "$REDEF_SAME_SCOPE_ERR" "redefinition of variable 'a'"

REDEF_TYPE_MUTATE_ERR="$($BIN -e 'int a = 1; string a = "hello";' 2>&1 || true)"
assert_contains "Type mutation via redefinition error" "$REDEF_TYPE_MUTATE_ERR" "redefinition of variable 'a'"

assert_eq "Lexical scoping (callee does not access caller locals)" \
    "$($BIN -e '
        int secret = 100;
        def foo() {
            return secret;
        }
        def bar() {
            int caller_local = 42;
            return foo();
        }
        print(bar());
    ')" \
    "100"

# 5. Compound Assignments and ++ / --
assert_eq "Compound assignments and increment/decrement" \
    "$($BIN -e '
        int a = 10;
        a += 5;
        a -= 3;
        a *= 2;
        a /= 4;
        a++;
        ++a;
        a--;
        print(a);
    ')" \
    "7"

# 6. Mutable List Indexing (1D and 2D) and Slicing
assert_eq "Mutable list indexing and slicing" \
    "$($BIN -e '
        list a = [1, 2, 3, 4, 5];
        a[0] = 10;
        a[-1] += 5;
        a[1]++;
        print(a, a[1:4], a[:2], a[3:]);
        list mat = [[1, 2], [3, 4]];
        mat[1][0] = 99;
        push(mat[0], 50);
        int popped = pop(mat[1]);
        print(mat[1][0], mat[0], popped);
    ')" \
    "[10, 3, 3, 4, 10] [3, 3, 4] [10, 3] [4, 10]
99 [1, 2, 50] 4"

# 7. String Escapes, Single Quotes, Indexing, and Slicing
assert_eq "String escapes and slicing" \
    "$($BIN -e '
        string s = "Hello\tWorld";
        string q = '\''KalamanC'\'';
        print(q[0], q[-1], q[0:7], q[7:]);
    ')" \
    "K C Kalaman C"

# 8. Foreach Loops (English 'in' and Filipino 'sa')
assert_eq "Foreach loops over list and string" \
    "$($BIN -e '
        int sum = 0;
        for (int v in range(1, 6)) sum += v;
        para (bilang v sa [10, 20]) sum += v;
        print(sum);
    ')" \
    "45"

# 9. Built-in Functions (negative to_int/to_double, push/pop, sort, reverse, join, min/max/abs/sqrt/pow_mod)
assert_eq "Built-in functions" \
    "$($BIN -e '
        print(to_int("-42") + 2, to_double("-3.5") * 2);
        list items = [3, 1, 4, 1, 5];
        push(items, 9);
        int last = pop(items);
        print(last, sort(items), reverse(items));
        print(join(["a", "b", "c"], "-"), abs(-15), min(items), max(items), pow_mod(2, 10, 1000), pow_mod(5000000000, 2, 6000000000));
    ')" \
    "-40 -7
9 [1, 1, 3, 4, 5] [5, 1, 4, 1, 3]
a-b-c 15 1 5 24 4000000000"

# 10. Matrix Multiplication, Exponentiation, and set_mod
assert_eq "Matrix operations and set_mod" \
    "$($BIN -e '
        list fib = [[1, 1], [1, 0]] ^ 10;
        print(fib[0][0], fib[0][1]);
        set_mod(10);
        list fib_mod10 = [[1, 1], [1, 0]] ^ 10;
        print(fib_mod10[0][0], fib_mod10[0][1]);
        set_mod(5000000003);
        list large_mat = [[4000000000]] * [[4000000000]];
        print(large_mat[0][0]);
    ')" \
    "89 55
9 5
400000006"

# 11. First-class Functions (storing user & built-in functions in variables)
assert_eq "First-class function variables" \
    "$($BIN -e '
        def square(x) return x * x;
        function f = square;
        function p = print;
        p(f(6));
    ')" \
    "36"

# 12. Parse Tree (#showtree / --tree) Dangling Pointer Fix
TREE_OUT="$($BIN --tree -e 'if (true) { int x = 1; } elif (false) { int x = 2; } else { int x = 3; }')"
assert_contains "Parse tree printing for if/elif/else" "$TREE_OUT" "SyntaxKind::IfExpression"

# 13. Interactive REPL Multi-line & Commands
REPL_OUT="$(printf 'def add(a, b) {\nreturn a + b;\n}\nadd(12, 30)\n#exit\n' | $BIN)"
assert_contains "REPL multi-line function and expression evaluation" "$REPL_OUT" "42"

# 14. Rich Error Diagnostics & Traceback
ERR_OUT="$($BIN -e '
def boom(x) {
    return 10 / x;
}
boom(0);
' || true)"
assert_contains "Division by zero error message" "$ERR_OUT" "ERROR: division by zero"
assert_contains "Function call traceback in error" "$ERR_OUT" "Traceback (most recent call last):"
assert_contains "Caret indicator in error output" "$ERR_OUT" "^"

# 15. Integer Overflow Safety (LLONG_MIN / -1, % -1, abs(LLONG_MIN))
OVERFLOW_DIV_ERR="$($BIN -e 'int m = -9223372036854775807 - 1; print(m / -1);' 2>&1 || true)"
assert_contains "LLONG_MIN division overflow error" "$OVERFLOW_DIV_ERR" "cannot divide LLONG_MIN by -1"

OVERFLOW_ABS_ERR="$($BIN -e 'int m = -9223372036854775807 - 1; print(abs(m));' 2>&1 || true)"
assert_contains "LLONG_MIN abs overflow error" "$OVERFLOW_ABS_ERR" "cannot negate LLONG_MIN"

assert_eq "LLONG_MIN % -1 evaluates to 0 without crash" \
    "$($BIN -e 'int m = -9223372036854775807 - 1; print(m % -1);')" \
    "0"

# 16. Dynamic Recursion Limit & Stack Overflow Guard
RECURSION_ERR="$($BIN -e 'set_recursion_limit(50); def f() { return f(); } f();' 2>&1 || true)"
assert_contains "Recursion depth limit error message" "$RECURSION_ERR" "maximum recursion depth exceeded (50)"
assert_contains "Recursion traceback frame omission" "$RECURSION_ERR" "frames omitted"

assert_eq "Dynamic recursion limit getters/setters (English & Filipino)" \
    "$($BIN -e '
        int orig = get_recursion_limit();
        set_recursion_limit(200);
        int mid = get_recursion_limit();
        itakda_lalim(300);
        int fin = kumuha_ng_lalim();
        print(orig, mid, fin);
    ')" \
    "1000 200 300"

assert_eq "CLI --recursion-limit flag configuration" \
    "$($BIN --recursion-limit=450 -e 'print(get_recursion_limit());')" \
    "450"

# 17. List Sorting Safety (strict weak ordering, mixed numbers, strings, empty, error on mixed types)
assert_eq "List sort functionality (strings, empty, mixed int/double)" \
    "$($BIN -e '
        print(sort(["banana", "apple", "cherry"]));
        print(sort([3.5, 1, 4.2, 2, 0.5]));
        print(sort([]));
    ')" \
    "['apple', 'banana', 'cherry']
[0.5, 1, 2, 3.5, 4.2]
[]"

SORT_ERR="$($BIN -e 'sort([1, "two", 3]);' 2>&1 || true)"
assert_contains "Sort incomparable types error" "$SORT_ERR" "cannot sort list with incomparable element types"

# 18. Control Flow Scope Validation (break/continue outside loops, return outside functions)
BREAK_ERR="$($BIN -e 'break;' 2>&1 || true)"
assert_contains "Break outside loop error" "$BREAK_ERR" "'break' outside of loop"

CONTINUE_ERR="$($BIN -e 'continue;' 2>&1 || true)"
assert_contains "Continue outside loop error" "$CONTINUE_ERR" "'continue' outside of loop"

RETURN_ERR="$($BIN -e 'return 42;' 2>&1 || true)"
assert_contains "Return outside function error" "$RETURN_ERR" "'return' outside of function"

FUNC_BREAK_ERR="$($BIN -e 'while (true) { def f() { break; } f(); }' 2>&1 || true)"
assert_contains "Break crossing function boundary error" "$FUNC_BREAK_ERR" "'break' outside of loop"

assert_eq "Valid break and continue inside loop execution" \
    "$($BIN -e 'int s = 0; for (int i in range(10)) { if (i == 3) continue; if (i == 7) break; s += i; } print(s);')" \
    "18"

# 19. Pure Keyword Lookup & Language Isolation
assert_eq "Language detection isolated to lexer and unchanged by runtime dispatch" \
    "$($BIN -e '
        bilang a = 1;
        lathala(a == 1);
        int val = to_int("42");
        lathala(a == 1);
    ')" \
    "totoo
totoo"

# 20. Standalone Block Scope Isolation
assert_eq "Standalone block lexical scope execution" \
    "$($BIN -e '
        int x = 5;
        {
            x = 10;
            int y = 20;
            print(x, y);
        }
        print(x);
    ')" \
    "10 20
10"

BLOCK_SHADOW_ERR="$($BIN -e 'int x = 5; { int x = 30; }' 2>&1 || true)"
assert_contains "Block shadowing outer variable error" "$BLOCK_SHADOW_ERR" "redefinition of variable 'x'"

BLOCK_LEAK_ERR="$($BIN -e '{ int y = 999; } print(y);' 2>&1 || true)"
assert_contains "Block local variable does not leak" "$BLOCK_LEAK_ERR" "undeclared identifier 'y'"

# 21. Function Parameter Type Enforcement
assert_eq "Function parameter type checking and dynamic polymorphism" \
    "$($BIN -e '
        def typed_add(int a, int b) { return a + b; }
        def dynamic_add(a, b) { return a + b; }
        print(typed_add(10, 25), dynamic_add("foo", "bar"));
    ')" \
    "35 foobar"

PARAM_TYPE_ERR="$($BIN -e 'def add(int a, int b) { return a + b; } add("hello", 2);' 2>&1 || true)"
assert_contains "Parameter type mismatch error" "$PARAM_TYPE_ERR" "unexpected type <string>, expected <int>"

# 22. Prefix vs Postfix Increment / Decrement Semantics
assert_eq "Prefix and postfix increment/decrement expressions" \
    "$($BIN -e '
        int a = 10;
        int post_inc = a++;
        int pre_inc = ++a;
        int post_dec = a--;
        int pre_dec = --a;
        list arr = [50];
        int arr_post = arr[0]++;
        int arr_pre = ++arr[0];
        print(post_inc, pre_inc, post_dec, pre_dec, a, arr_post, arr_pre, arr[0]);
    ')" \
    "10 12 12 10 10 50 52 52"

# 23. String Split Semantics (whitespace vs custom delimiter empty tokens)
assert_eq "String split whitespace collapsing vs custom delimiter preservation" \
    "$($BIN -e '
        print(split("  hello \t world \n "));
        print(split("a,,b", ","));
        print(split("a,b,", ","));
        print(split(",", ","));
    ')" \
    "['hello', 'world']
['a', '', 'b']
['a', 'b', '']
['', '']"

SPLIT_EMPTY_SEP_ERR="$($BIN -e 'split("hello", "");' 2>&1 || true)"
assert_contains "Split empty separator error" "$SPLIT_EMPTY_SEP_ERR" "empty separator in split()"

# 24. CLI Script Arguments & Positional Parsing
assert_eq "CLI arguments passed via -e and accessible via args and mga_argumento" \
    "$($BIN -e 'print(args);' foo bar baz)" \
    "['foo', 'bar', 'baz']"

assert_eq "CLI arguments in Filipino accessible via mga_argumento" \
    "$($BIN -e 'lathala(mga_argumento);' uno dos)" \
    "['uno', 'dos']"

assert_eq "CLI script argument flag passthrough after --" \
    "$($BIN -e 'print(args);' -- -v --custom 123)" \
    "['-v', '--custom', '123']"

TEMP_SCRIPT="$(mktemp /tmp/kalman_test_args_XXXXXX.kal)"
echo 'print("len:", len(args), "first:", args[0]);' > "$TEMP_SCRIPT"
assert_eq "CLI script file arguments do not overwrite script path" \
    "$($BIN "$TEMP_SCRIPT" first_arg second_arg -v)" \
    "len: 3 first: first_arg"

assert_eq "CLI -- option terminator passes script arguments" \
    "$($BIN -- "$TEMP_SCRIPT" alpha beta)" \
    "len: 2 first: alpha"
rm -f "$TEMP_SCRIPT"

# 25. Strict List Concatenation (+ requires list operands)
assert_eq "List concatenation with list operand" \
    "$($BIN -e 'print([1, 2] + [3, 4]);')" \
    "[1, 2, 3, 4]"

LIST_INT_ADD_ERR="$($BIN -e 'print([1, 2] + 3);' 2>&1 || true)"
assert_contains "List + non-list addition error" "$LIST_INT_ADD_ERR" "illegal operation <+> between <list> and <int>"

INT_LIST_ADD_ERR="$($BIN -e 'print(3 + [1, 2]);' 2>&1 || true)"
assert_contains "Non-list + list addition error" "$INT_LIST_ADD_ERR" "illegal operation <+> between <int> and <list>"

# 26. Zero-Copy Index Read & Direct Chain Traversal
assert_eq "Zero-copy index access in large loop" \
    "$($BIN -e '
        list a = range(20000);
        int s = 0;
        for (int i in range(20000)) {
            s += a[i];
        }
        print(s);
    ')" \
    "199990000"

assert_eq "Chained index access on temporary container and side-effect safety" \
    "$($BIN -e '
        def get_matrix() { return [[10, 20], [30, 40]]; }
        int t = get_matrix()[1][0];
        list a = [1, 2, 3];
        def mutate() { a = [999]; return 0; }
        int m = a[mutate()];
        print(t, m);
    ')" \
    "30 999"

# 27. Foreach Single-Copy Ownership Transfer
assert_eq "Foreach over list and range with break and continue" \
    "$($BIN -e '
        int s = 0;
        for (int x in range(100)) {
            if (x < 10) continue;
            if (x >= 20) break;
            s += x;
        }
        list words = ["hello", "brave", "new", "world"];
        string joined = "";
        for (string w in words) {
            joined += w + " ";
        }
        print(s, joined);
    ')" \
    "145 hello brave new world "

# 28. Flat Buffer Matrix Exponentiation
assert_eq "Flat buffer matrix exponentiation identity and large power" \
    "$($BIN -e '
        list T = [[1, 1], [1, 0]];
        print(T ^ 0);
        set_mod(1000000007);
        list big = T ^ 50;
        print(big[0][1]);
    ')" \
    "[[1, 0],
 [0, 1]]
586268941"

# 29. Zero-Copy Function Invocation
assert_eq "High-volume user and builtin function calls without copies" \
    "$($BIN -e '
        def square(int x) { return x * x; }
        int s = 0;
        for (int i in range(1000)) {
            s += square(len(to_string(i)));
        }
        print(s);
    ')" \
    "8470"

# 30. Fast Numeric Literal Parsing & Conversion
assert_eq "Fast strtoll and strtold numeric literal and conversion parsing" \
    "$($BIN -e '
        int a = 9223372036854775806;
        double b = 123456.789;
        int c = to_int("-98765");
        double d = to_double("42.5");
        print(a, b, c, d);
    ')" \
    "9223372036854775806 123456.789 -98765 42.5"

# 31. Direct Initializer in VarDeclareExpression
assert_eq "Direct initialized and uninitialized variable declarations" \
    "$($BIN -e '
        int x = 42;
        int uninit_int;
        double y = 3.14;
        double uninit_dbl;
        string s = "kalaman";
        string uninit_str;
        bool b = true;
        bool uninit_b;
        list l = [1, 2, 3];
        list uninit_l;
        print(x, uninit_int, y, uninit_dbl, s, uninit_str, b, uninit_b, l, uninit_l);
    ')" \
    "42 0 3.14 0 kalaman  true false [1, 2, 3] []"

INIT_TYPE_ERR="$($BIN -e 'int x = "invalid";' 2>&1 || true)"
assert_contains "Type mismatch error on variable declaration initializer" "$INIT_TYPE_ERR" "invalid assignment of <string> to <int>"

# 32. Compile-Time Constexpr Constant Resolution
assert_eq "Constexpr bilingual keywords and builtins resolution" \
    "$($BIN -e '
        aka bati(mgawords pangalan) {
            ibalik "Kumusta, " + pangalan;
        }
        lathala(bati("Mundo"));
        lathala(totoo, mali);
    ')" \
    "Kumusta, Mundo
totoo mali"

# 33. In-Place Self-Doubling String and List Repetition
assert_eq "In-place string and list repetition" \
    "$($BIN -e '
        string s1 = "abc" * 7;
        string s2 = 4 * "xyz";
        string s3 = "kalaman" * 0;
        string s4 = "test" * 1;
        list l1 = [1, 2] * 3;
        list l2 = 2 * ["a", "b"];
        list l3 = [9] * 0;
        print(s1, s2, s3, s4, l1, l2, l3);
    ')" \
    "abcabcabcabcabcabcabc xyzxyzxyzxyz  test [1, 2, 1, 2, 1, 2] ['a', 'b', 'a', 'b'] []"

NEG_STR_ERR="$($BIN -e '"abc" * -5;' 2>&1 || true)"
assert_contains "Negative string multiplier error" "$NEG_STR_ERR" "illegal operation <*> between <string> and <int>"

NEG_LIST_ERR="$($BIN -e '[1, 2] * -1;' 2>&1 || true)"
assert_contains "Negative list multiplier error" "$NEG_LIST_ERR" "illegal operation <*> between <list> and <int>"

# 34. List and Matrix to_string formatting
assert_eq "List and matrix to_string formatting" \
    "$($BIN -e '
        list empty = [];
        list one_d = [1, 2, 3];
        list mat = [[1, 2], [3, 4]];
        list ragged = [[1, 2], [3]];
        list str_2d = [["a", "b"], ["c", "d"]];
        print(empty);
        print(one_d);
        print(mat);
        print(ragged);
        print(str_2d);
    ')" \
    "[]
[1, 2, 3]
[[1, 2],
 [3, 4]]
[[1, 2], [3]]
[['a', 'b'], ['c', 'd']]"

echo ""
echo "Summary: $PASS passed, $FAIL failed."
if [ "$FAIL" -ne 0 ]; then
    exit 1
fi
