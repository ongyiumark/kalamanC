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

# 4. Variable Scoping & Recursive Shadowing Bug Fix
assert_eq "Local variable shadowing in functions & recursion" \
    "$($BIN -e '
        int x = 100;
        def rec(int n) {
            int x = n * 10;
            if (n <= 1) return x;
            return x + rec(n - 1);
        }
        print(rec(3), x);
    ')" \
    "60 100"

assert_eq "Lexical scoping (callee does not access caller locals)" \
    "$($BIN -e '
        int secret = 100;
        def foo() {
            return secret;
        }
        def bar() {
            int secret = 42;
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
        print(join(["a", "b", "c"], "-"), abs(-15), min(items), max(items), pow_mod(2, 10, 1000));
    ')" \
    "-40 -7
9 [1, 1, 3, 4, 5] [5, 1, 4, 1, 3]
a-b-c 15 1 5 24"

# 10. Matrix Multiplication, Exponentiation, and set_mod
assert_eq "Matrix operations and set_mod" \
    "$($BIN -e '
        list fib = [[1, 1], [1, 0]] ^ 10;
        print(fib[0][0], fib[0][1]);
        set_mod(10);
        list fib_mod10 = [[1, 1], [1, 0]] ^ 10;
        print(fib_mod10[0][0], fib_mod10[0][1]);
    ')" \
    "89 55
9 5"

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

echo ""
echo "Summary: $PASS passed, $FAIL failed."
if [ "$FAIL" -ne 0 ]; then
    exit 1
fi
