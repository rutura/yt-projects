#include <print>
#include <stdexcept>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 07 - Exceptions & breaking on the throw site
//
// Note: this scenario's setup step depends on which debugger backend your
// kit uses. If your kit is MSVC (visible as "MSVC 2022" in your kit's
// name), Qt Creator debugs it with CDB, and exception breaking is a
// global preference rather than a per-breakpoint option:
//   * Go to Edit > Preferences > Debugger > CDB.
//   * In the "Break On" group, enable the option for C++ exceptions
//     (also enable "Add Exceptions to Issues View" if you want a written
//     record in the Issues pane).
// If your kit uses GDB or LLDB (typical for MinGW/Linux/macOS kits)
// instead, add a breakpoint whose type is specifically an exception
// breakpoint from the Breakpoints view's "Add Breakpoint" dialog instead
// of the steps above.
//
// What to try in Qt Creator:
//   * With exception breaking enabled as above, start debugging (F5) and
//     run this scenario. Execution stops the instant
//     `throw std::invalid_argument(...)` runs inside divide() -- not at
//     the `catch` block -- so you see exactly where and why it was
//     thrown, with the full call stack back to where divide() was called
//     from.
//   * Continue (F5). Now set a normal breakpoint inside the `catch`
//     block (the `std::println("Caught expected exception: {}", ...)`
//     line) and inspect the caught object `e` in the Locals view --
//     expand it to see the message that `.what()` returns stored inside.
//   * Disable the CDB "Break On" C++ exceptions option (or remove the
//     GDB/LLDB exception breakpoint) and re-run: this time execution
//     jumps straight to the `catch` block with no pause at the throw
//     site. Notice how much less you know about where the exception
//     actually came from -- this is exactly why exception breakpoints
//     exist for bugs in bigger codebases.
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

} // namespace

void scenarios::run_exceptions() {
    std::println("-- handled exception demo --");
    handled_exception_demo();

    std::println("(Uncaught-exception crash demo is intentionally not run");
    std::println(" automatically. See README.md Scenario 6 for how to");
    std::println(" trigger it safely under the debugger.)");
}
