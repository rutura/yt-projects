#include <print>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 09 - Conditional & logging breakpoints in a hot loop
//
// The loop below runs 1000 iterations and has a deliberately planted bug:
// exactly once, when `i == 733`, `divisor` becomes 0 right before the
// division, which would crash the program. Finding iteration 733 by
// stepping 733 times with F10 would be painful. This is exactly what
// conditional breakpoints are for.
//
// What to try in Qt Creator:
//   * Set a breakpoint on the `int result = value / divisor;` line.
//   * Right-click the breakpoint (in the editor gutter or the Breakpoints
//     view) -> "Edit Breakpoint..." and set the condition to `divisor == 0`.
//     Re-run: the debugger now skips straight to the one iteration that
//     matters instead of stopping 1000 times.
//   * Alternatively, set the condition to `i == 733` to jump to the exact
//     iteration by index.
//   * Try a *logging* (tracepoint) breakpoint instead: Edit Breakpoint ->
//     enable "message" logging with text like `i={i} divisor={divisor}`
//     and untick "stop on hit". Re-run and watch the Application Output /
//     Debugger Console print a trace line per iteration without ever
//     halting -- useful for spotting patterns without single-stepping.
//   * Also try setting an ignore count (right-click -> "Skip N times")
//     of 732 on a plain, unconditional breakpoint at the same line, which
//     achieves the same jump-to-733 result a different way.
// ---------------------------------------------------------------------------

namespace {

int risky_divisor_for(int i) {
    // Bug: for i == 733 this returns 0, which would trigger a division by
    // zero at the call site below. Every other iteration returns a safe,
    // non-zero divisor.
    if (i == 733) {
        return 0;
    }
    return (i % 17) + 1;
}

} // namespace

void scenarios::run_conditional_breakpoints() {
    std::vector<int> results;
    results.reserve(1000);

    for (int i = 0; i < 1000; ++i) {
        const int value = i * 3;
        const int divisor = risky_divisor_for(i);

        if (divisor == 0) {
            // Guarded here so the demo doesn't actually crash when run
            // without the debugger. Set your conditional breakpoint on
            // the line below (inside the guard is fine too) to catch the
            // moment `divisor` would have been zero.
            std::println("Skipping i={} to avoid division by zero (divisor was 0)", i);
            continue;
        }

        int result = value / divisor; // <-- conditional breakpoint here: divisor == 0
        results.push_back(result);
    }

    std::println("Processed {} safe results out of 1000 iterations", results.size());
}
