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
//   * Set a breakpoint on the `int result = value / divisor;` line
//     (click the left margin, or F9 with your cursor there).
//   * Right-click that breakpoint (either its red dot in the editor
//     margin, or its row in the Breakpoints view) and choose
//     "Edit Breakpoint...". In the dialog, find the "Condition" field
//     and enter `divisor == 0`. Start debugging (F5): the debugger now
//     skips straight to the one iteration where that's true, instead of
//     stopping 1000 times.
//   * Alternatively, set the condition to `i == 733` to jump to the
//     exact loop iteration by index instead.
//   * Try the same dialog's other fields instead: clear the condition,
//     and set "Ignore count" to `732` on this same breakpoint. This
//     tells the debugger to silently skip the first 732 hits and only
//     actually stop on the 733rd -- a different way of reaching the
//     same iteration.
//   * Try a *tracepoint* (logging breakpoint) instead of stopping at
//     all: in the same "Edit Breakpoint..." dialog, check
//     "Tracepoint only" and put something like `i={i} divisor={divisor}`
//     in the "Message" field. Start debugging again and watch the
//     Application Output / Debugger Console print one line per
//     iteration without the program ever actually pausing -- useful for
//     spotting a pattern across many iterations without single-stepping
//     through all of them.
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
