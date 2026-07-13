#include <print>
#include <stdexcept>
#include <string>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 06 - Exceptions & the exception breakpoint
//
// What to try in Qt Creator:
//   * Before running, open Debug > Breakpoints (or the Breakpoints view)
//     and add a "C++ Exception" breakpoint (right-click in the Breakpoints
//     pane -> Add Breakpoint... -> Exception). This pauses execution the
//     instant *any* C++ exception is thrown, before any catch/unwind.
//   * Run this scenario. With the exception breakpoint set, you'll land
//     directly inside divide() at the `throw std::invalid_argument(...)`
//     line -- not at the catch block -- so you see the true throw site.
//   * Remove the exception breakpoint and re-run: now execution just skips
//     straight to the `catch` block, and the original throw location is
//     lost from view (only the Stack view's unwind history hints at it).
//   * Inspect the caught exception object `e` in Locals inside the catch
//     block, expanding it to see `what()`'s stored message.
//   * Try the uncaught_exception_demo() path (see README) to see how Qt
//     Creator stops the whole program on an unhandled exception.
// ---------------------------------------------------------------------------

namespace {

double divide(double numerator, double denominator) {
    if (denominator == 0.0) {
        // <-- exception breakpoint stops here
        throw std::invalid_argument("division by zero in divide()");
    }
    return numerator / denominator;
}

void handled_exception_demo() {
    try {
        const double result = divide(10.0, 0.0);
        std::println("result = {}", result); // never reached
    } catch (const std::invalid_argument& e) {
        // <-- normal breakpoint here to inspect `e`
        std::println("Caught expected exception: {}", e.what());
    }
}

void nested_rethrow_demo() {
    try {
        try {
            throw std::runtime_error("inner failure");
        } catch (const std::runtime_error& inner) {
            std::println("Inner catch saw: {}", inner.what());
            throw; // rethrow: watch the Stack view during the second unwind
        }
    } catch (const std::exception& outer) {
        std::println("Outer catch saw: {}", outer.what());
    }
}

} // namespace

void scenarios::run_exceptions() {
    std::println("-- handled exception demo --");
    handled_exception_demo();

    std::println("-- nested catch/rethrow demo --");
    nested_rethrow_demo();

    std::println("(Uncaught-exception crash demo is intentionally not run");
    std::println(" automatically. See README.md Scenario 6 for how to");
    std::println(" trigger it safely under the debugger.)");
}
