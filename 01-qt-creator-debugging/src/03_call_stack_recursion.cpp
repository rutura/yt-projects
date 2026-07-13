#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 03 - Call stack & recursion
//
// What to try in Qt Creator:
//   * Set a breakpoint inside fibonacci() on the
//     `return fibonacci(n - 1) + fibonacci(n - 2);` line.
//   * Start debugging (F5). When it stops, look for the "Stack" view
//     (usually visible by default in the Debug mode layout, on the
//     right side). If it's not visible, go to View > Views > Stack.
//   * Continue (F5) a few times and watch the Stack view: it grows by
//     one entry every time fibonacci() calls itself, because each call
//     is a new, still-unfinished frame sitting on top of the last one.
//     Click on different entries in the Stack view to jump to that
//     frame -- the Locals view updates to show `n`'s value at that
//     specific level of recursion.
//   * Instead of Continue, try Step Out (Shift+F11) repeatedly: each
//     press finishes the current (innermost) fibonacci() call and pops
//     you back to the frame that called it, one level at a time, and you
//     can watch the Stack view shrink back down.
//   * Once you've done that, restart and set a breakpoint inside
//     level_three() instead, then step through deeply_nested_call(). This
//     stack is not recursive: it's four distinctly-named functions
//     calling into each other, which is closer to what you'll see when
//     debugging real, non-recursive code.
// ---------------------------------------------------------------------------

namespace {

long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2); // <-- breakpoint candidate
}

int level_three(int x) {
    int result = x * x;
    return result; // <-- breakpoint candidate, inspect the stack here
}

int level_two(int x) {
    return level_three(x + 1);
}

int level_one(int x) {
    return level_two(x * 2);
}

int deeply_nested_call(int x) {
    return level_one(x);
}

} // namespace

void scenarios::run_call_stack_recursion() {
    std::println("Computing fibonacci(10) recursively (watch the Stack view grow)...");
    const long long fib = fibonacci(10);
    std::println("fibonacci(10) = {}", fib);

    std::println("Now a non-recursive but deep call chain...");
    const int nested = deeply_nested_call(4);
    std::println("deeply_nested_call(4) = {}", nested);
}
