#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 10 - Debug vs. Release: optimized-build debugging quirks
//
// Every other scenario runs from the same Debug build. This one is
// designed to be compared across TWO separate builds of this project:
//   1. A normal Debug build (-DCMAKE_BUILD_TYPE=Debug), unoptimized, full
//      debug info -- how you've built everything else so far.
//   2. A RelWithDebInfo build (-DCMAKE_BUILD_TYPE=RelWithDebInfo),
//      optimizations on but debug info still generated. In Qt Creator,
//      add a second "Build" configuration for this project using that
//      build type (or a second Kit configured with it), so you can run
//      and debug both side by side.
//
// See README.md "Scenario 10" for the exact steps to set up the second
// build configuration in Qt Creator.
//
// What to try in Qt Creator:
//   * In the Debug build, step through inefficient_sum() line by line
//     with F10: every loop iteration and the `subtotal` temporary are
//     visible exactly as written.
//   * In the RelWithDebInfo build, set the SAME breakpoint on the
//     `subtotal +=` line. You'll likely see it hit far fewer times than
//     expected, or the debugger may report a variable as "<optimized
//     out>" in Locals -- the compiler inlined/unrolled the loop or kept
//     `subtotal` purely in a register with no stable memory address.
//     This is one of the most common "wait, why doesn't the debugger
//     match my code?" moments students hit in real projects.
//   * Compare the Disassembly view (Debug > Windows > Disassembly, or
//     right-click in the editor -> "Show Disassembly") between the two
//     builds for the same function to see the instruction count differ
//     drastically.
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
