#include <array>
#include <print>
#include <string_view>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 04 - Conditional breakpoints
//
// We want a breakpoint that only triggers when a specific condition is
// met, instead of stopping on every hit. Below, the same line runs once
// per order, with a different `quantity` and `unit_price` each time. A
// plain breakpoint there stops on every order; a conditional breakpoint
// lets you stop only on the one you actually care about.
//
// See README.md, Scenario 4, for the full walkthrough.
// ---------------------------------------------------------------------------

void scenarios::run_conditional_breakpoints() {
    struct Order {
        std::string_view customer;
        int quantity;
        int unit_price;
    };

    const std::array<Order, 5> orders{{
        {"Aria",   2, 15},
        {"Beno",   1, 40},
        {"Chidi",  20, 5},
        {"Dax",    3, 0},
        {"Elin",   4, 12},
    }};

    for (const Order& order : orders) {
        const int quantity = order.quantity;
        const int unit_price = order.unit_price;

        const int total = quantity * unit_price; // <-- breakpoint here

        std::println("{}: {} x {} = {}", order.customer, quantity, unit_price, total);
    }
}
