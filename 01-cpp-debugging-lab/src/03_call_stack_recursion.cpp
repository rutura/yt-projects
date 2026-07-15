#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 03 - Call stack & recursion
// See README.md, Scenario 3, for the full walkthrough.
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
