#include <concepts>
#include <print>
#include <string>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 08 - Templates & generic code (C++23)
//
// What to try in Qt Creator:
//   * Set a breakpoint inside the body of `sum_all`. Because it's a
//     template, the debugger will only show you the *instantiations*
//     actually used below (int and double). Step in from each call site
//     and check the Locals view / the function signature shown in the
//     Stack view -- it will display the concrete instantiated type
//     (e.g. `sum_all<int>`), not the generic template.
//   * Set a breakpoint inside Box<T>::describe() (a member function that
//     uses C++23's "deducing this"). Inspect `self` in Locals -- it plays
//     the role of both const and non-const `this` depending on how it's
//     called.
//   * Step through `classify()` which uses `if consteval`: notice the
//     debugger only ever shows you the runtime branch, because the
//     compile-time branch (if selected) wouldn't exist as debuggable code
//     at all when evaluated at compile time.
// ---------------------------------------------------------------------------

namespace {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
T sum_all(const std::vector<T>& values) {
    T total{};
    for (const T& v : values) {
        total += v; // <-- breakpoint here; check the instantiated type in Stack view
    }
    return total;
}

template <typename T>
struct Box {
    T value;

    // C++23 "deducing this": one function body serves both const and
    // non-const call sites via the deduced `self` parameter.
    template <typename Self>
    std::string describe(this Self&& self) {
        return std::format("Box holds: {}", self.value); // <-- breakpoint here, inspect `self`
    }
};

constexpr int classify(int n) {
    if consteval {
        // Only taken during compile-time evaluation; never hit by the
        // runtime debugger.
        return n < 0 ? -1 : 1;
    } else {
        // Runtime path: this is the branch you'll actually be able to
        // step into with a live debugger session.
        return n < 0 ? -1 : (n == 0 ? 0 : 1); // <-- breakpoint here
    }
}

} // namespace

void scenarios::run_templates_generic() {
    std::vector<int> ints{1, 2, 3, 4, 5};
    std::vector<double> doubles{1.5, 2.5, 3.0};

    const int int_sum = sum_all(ints);       // instantiates sum_all<int>
    const double double_sum = sum_all(doubles); // instantiates sum_all<double>
    std::println("int_sum = {}, double_sum = {}", int_sum, double_sum);

    Box<std::string> box{.value = "hello templates"};
    std::println("{}", box.describe());

    const int runtime_value = 7; // not a compile-time constant to the optimizer here
    const int classification = classify(runtime_value);
    std::println("classify({}) = {}", runtime_value, classification);
}
