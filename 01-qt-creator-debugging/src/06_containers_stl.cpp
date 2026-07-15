#include <algorithm>
#include <map>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 06 - STL containers & pretty-printers
//
// What to try in Qt Creator:
//   * Set a breakpoint on the `std::optional<int> maybe_missing = 42;`
//     line (this is after `scores`, `name_to_id`, and `fast_lookup` are
//     already filled in, so all three are visible in Locals).
//   * Expand `scores` (a std::vector<int>) in the Locals view: Qt
//     Creator's debugger doesn't show you the vector's raw internal
//     pointers, it shows a clean, indexed list of the actual int values --
//     this is called a "pretty-printer."
//   * Expand `name_to_id` (a std::map<std::string,int>) and `fast_lookup`
//     (a std::unordered_map<std::string,int>) the same way, and compare
//     how each renders its key/value pairs.
//   * Step over (F10) to the `maybe_missing.reset();` line. Look at
//     `maybe_missing` (a std::optional<int>) in Locals before you step
//     over that line, then again after: the pretty-printer shows a clear
//     "has a value" vs. "empty" state, and you watch it flip live.
//   * Set a breakpoint inside the `for (int s : high_scores)` loop, on
//     the `std::print("{} ", s);` line. Step through it (F10) one
//     iteration at a time. `high_scores` is a std::ranges pipeline
//     (filter + transform) -- unlike `scores`, it isn't a real container
//     sitting in memory; it computes each value lazily as you iterate,
//     which is why stepping through this loop is a good way to see that
//     difference play out one value at a time.
// ---------------------------------------------------------------------------

namespace {

std::vector<int> make_scores() {
    return {42, 88, 15, 91, 67, 30, 99, 5};
}

} // namespace

void scenarios::run_containers_stl() {
    std::vector<int> scores = make_scores();

    std::map<std::string, int> name_to_id{
        {"alice", 1}, {"bob", 2}, {"carol", 3}
    };

    std::unordered_map<std::string, int> fast_lookup{
        {"alice", 100}, {"bob", 200}, {"carol", 300}
    };

    // <-- breakpoint here, inspect all four containers
    std::optional<int> maybe_missing = 42;

    std::println("scores has {} entries", scores.size());
    std::println("name_to_id has {} entries", name_to_id.size());
    std::println("fast_lookup has {} entries", fast_lookup.size());
    std::println("maybe_missing = {}", maybe_missing.has_value() ? "has value" : "empty");

    maybe_missing.reset(); // <-- step here, re-inspect maybe_missing: now "empty"
    std::println("maybe_missing after reset = {}",
                  maybe_missing.has_value() ? "has value" : "empty");

    // C++23-flavored ranges pipeline: filter + transform, lazily evaluated.
    auto high_scores = scores
                      | std::views::filter([](int s) { return s >= 50; })
                      | std::views::transform([](int s) { return s + 5; });

    std::print("high_scores (bonus applied): ");
    for (int s : high_scores) {
        std::print("{} ", s); // <-- breakpoint here to single-step the lazy pipeline
    }
    std::println("");

    std::ranges::sort(scores);
    std::print("sorted scores: ");
    for (int s : scores) {
        std::print("{} ", s);
    }
    std::println("");
}
