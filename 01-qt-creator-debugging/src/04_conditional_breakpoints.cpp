#include <print>

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
// What to try in Qt Creator:
//   * Set a breakpoint on the `const int total = quantity * unit_price;`
//     line (click the left margin, or F9 with your cursor there).
//   * Start debugging (F5) and pick this scenario. Notice it stops on
//     every single order.
//   * Stop the session. Right-click that breakpoint (its red dot in the
//     editor margin, or its row in the Breakpoints view) and choose
//     "Edit Breakpoint...". In the dialog, find the "Condition" field and
//     enter `quantity > 10`. Debug again: the debugger skips straight to
//     the one order where that's true, instead of stopping on all of
//     them.
//   * Try a different condition instead, such as `unit_price == 0`, to
//     land on the free/comped order.
// ---------------------------------------------------------------------------

void scenarios::run_conditional_breakpoints() {
    struct Order {
        const char* customer;
        int quantity;
        int unit_price;
    };

    const Order orders[] = {
        {"Aria",   2, 15},
        {"Beno",   1, 40},
        {"Chidi",  20, 5},
        {"Dax",    3, 0},
        {"Elin",   4, 12},
    };

    for (const Order& order : orders) {
        const int quantity = order.quantity;
        const int unit_price = order.unit_price;

        const int total = quantity * unit_price; // <-- breakpoint here

        std::println("{}: {} x {} = {}", order.customer, quantity, unit_price, total);
    }
}
