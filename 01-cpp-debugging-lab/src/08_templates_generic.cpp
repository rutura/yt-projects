#include <concepts>
#include <print>
#include <string>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 08 - Templates & generic code (C++23)
//
// A template isn't compiled code by itself -- it's a *blueprint* the
// compiler stamps out into a real function or class only once you use it
// with a specific type. sum_all<T> below gets called with a
// vector<int> and a vector<double>, so the compiler quietly generates
// two entirely separate functions, sum_all<int> and sum_all<double>,
// from the one template definition:
//
//     template sum_all<T>(blueprint)
//            |
//            |-- used with vector<int>    --> compiles sum_all<int>
//            |-- used with vector<double> --> compiles sum_all<double>
//
// That's why a breakpoint set on one single source line inside sum_all()
// can report two different instantiated function names -- see README.md.
//
// `Numeric` is a *concept* (C++20/23): a named, checkable constraint on
// what a template argument is allowed to be (`std::integral<T> ||
// std::floating_point<T>`). It doesn't change what gets generated the
// way the type itself does -- it just makes the compiler reject
// `sum_all<std::string>` at compile time with a readable error, instead
// of failing deep inside the function body with a confusing one.
//
// `if consteval` picks between two function bodies depending on *when*
// the call happens, not what type is involved: the same classify()
// function has one body for compile-time evaluation and a different one
// for runtime. Only the runtime body is ever real, steppable code in a
// live debugger -- the compile-time body already finished running
// during compilation, before your program even started.
//
// Full explanation with diagrams, plus step-by-step debugger
// instructions, is in the project README.md under "Scenario 8 --
// Templates & generic code".
// ---------------------------------------------------------------------------

namespace {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
T sum_all(const std::vector<T>& values) {
    T total{};
    for (const T& v : values) {
        total += v; // <-- breakpoint here; check the instantiated type in the call stack
    }
    return total;
}

template <typename T>
struct Box {
    T value;

    std::string describe() const {
        // <-- breakpoint here, inspect `this`
        return std::format("Box holds: {}", value);
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

    constexpr int runtime_value = 7;
    const int classification = classify(runtime_value);
    std::println("classify({}) = {}", runtime_value, classification);
}
