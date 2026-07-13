#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 10 - Debug vs. Release: optimized-build debugging quirks
//
// Every other scenario runs from the same Debug build. This one is
// designed to be compared across TWO separate builds of this project:
//   1. A normal Debug build (CMAKE_BUILD_TYPE=Debug), unoptimized, full
//      debug info -- how you've built everything else so far.
//   2. A RelWithDebInfo build (CMAKE_BUILD_TYPE=RelWithDebInfo),
//      optimizations on but debug info still generated.
//
// See README.md "Scenario 10" for the exact steps to add and switch to a
// second build configuration in Qt Creator's Projects mode.
//
// What to try in Qt Creator:
//   * With the Debug build active, set a breakpoint on the
//     `subtotal += j;` line and step through inefficient_sum() with F10:
//     every loop iteration and the `subtotal` temporary are visible
//     exactly as written, updating one step at a time.
//   * Switch to the RelWithDebInfo build (see README.md "Scenario 10"),
//     rebuild, and set the SAME breakpoint on the same
//     `subtotal += j;` line. Step through it again: you'll likely see it
//     get hit far fewer times than expected, or the Locals view may
//     report `subtotal` as "<optimized out>" or "not in scope" -- the
//     compiler inlined or unrolled the loop, or kept `subtotal` purely
//     in a CPU register with no stable memory location for the debugger
//     to point at. This is one of the most common "wait, why doesn't
//     the debugger match my code?" moments you'll hit in real projects
//     once you start debugging Release builds.
//   * While stopped, go to Debug > Operate by Instruction (or the
//     matching button on the debugger toolbar) to switch the editor
//     into a Disassembler view of the current function's actual machine
//     code. Do this in both builds and compare: the Debug build's
//     disassembly closely mirrors your C++ line by line, while the
//     RelWithDebInfo build's is usually shorter and reordered.
// ---------------------------------------------------------------------------

namespace {

long long inefficient_sum(int n) {
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        long long subtotal = 0;
        for (int j = 0; j <= i; ++j) {
            // <-- set a breakpoint here in BOTH builds and compare hit behavior
            subtotal += j;
        }
        total += subtotal;
    }
    return total;
}

} // namespace

void scenarios::run_optimized_release_debugging() {
    std::println("Running inefficient_sum(2000)... compare this scenario between a");
    std::println("Debug build and a RelWithDebInfo build of this same project (see");
    std::println("README.md Scenario 10 for how to set up the second build in Qt");
    std::println("Creator).");

    const long long result = inefficient_sum(2000);
    std::println("inefficient_sum(2000) = {}", result);
}
