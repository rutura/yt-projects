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
// See README.md, Scenario 6, for the full walkthrough.
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
