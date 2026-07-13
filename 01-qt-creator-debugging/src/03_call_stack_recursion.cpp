#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 03 - Call stack & recursion
//
// What to try in Qt Creator:
//   * Set a breakpoint inside fibonacci() on the `return fibonacci(n - 1)...`
//     line.
//   * Run the debugger and open View > Debugger Windows > Stack.
//   * Watch the stack grow one frame per recursive call; click through
//     frames to see `n` change at each level in the Locals view.
//   * Try "Step Out" (Shift+F11) repeatedly to unwind frame by frame and
//     watch return values.
//   * Set a breakpoint inside deeply_nested_call() to see a *mixed* stack:
//     several distinctly-named functions calling into each other, which is
//     more representative of real code than pure recursion.
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
