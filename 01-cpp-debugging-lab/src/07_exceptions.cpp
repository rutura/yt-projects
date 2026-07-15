#include <print>
#include <stdexcept>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 07 - Exceptions & breaking on the throw site
// See README.md, Scenario 7, for the full walkthrough.
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
    std::println(" automatically. See README.md Scenario 7 for how to");
    std::println(" trigger it safely under the debugger.)");
}
