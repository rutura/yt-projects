#include <print>
#include <string_view>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 01 - Breakpoints & stepping
//
// This is the very first scenario: if this is your first time using a
// debugger, start here. Full walkthrough in README.md "Scenario 1" -- the
// short version:
//   * Click in the editor's left margin (the thin strip left of the line
//     numbers) on the `double price = base_price(item);` line, OR put
//     your cursor on that line and press F9. A red dot appears: that's a
//     breakpoint.
//   * Start debugging: press F5 (or Debug > Start Debugging). Qt Creator
//     builds the project if needed, launches it, and the console window
//     runs until it reaches your breakpoint, where it pauses.
//   * Press F10 (Step Over) repeatedly to walk through apply_discount()
//     and add_tax() one call at a time, without diving inside them.
//   * Restart (Shift+F5 to stop, F5 to start again), stop at the same
//     breakpoint, and this time press F11 (Step Into) on the
//     compute_receipt() call to actually step inside it.
//   * Once inside add_tax(), press Shift+F11 (Step Out) to run the rest
//     of that function and pop back up to whatever called it.
// ---------------------------------------------------------------------------

namespace {

double base_price(std::string_view item) {
    if (item == "widget") return 9.99;
    if (item == "gadget") return 24.50;
    return 4.99; // fallback item, deliberately vague on purpose
}

double apply_discount(double price, double percent_off) {
    const double discount = price * (percent_off / 100.0);
    return price - discount;
}

double add_tax(double price, double tax_rate) {
    return price + (price * tax_rate);
}

double compute_receipt(std::string_view item, double percent_off, double tax_rate) {
    int total_steps = 0; // step through and watch this increment

    double price = base_price(item);
    ++total_steps;

    double discounted = apply_discount(price, percent_off);
    ++total_steps;

    double final_price = add_tax(discounted, tax_rate);
    ++total_steps;

    return final_price;
}

} // namespace

void scenarios::run_breakpoints_stepping() {
    std::println("Computing a small receipt so you have something to step through...");

    const int total = static_cast<int>(compute_receipt("widget", 10.0, 0.08) * 100);
    std::println("Final price (cents): {}", total);

    std::println("Now do the same for a bogus item to see the fallback branch:");
    const int total2 = static_cast<int>(compute_receipt("mystery-item", 0.0, 0.08) * 100);
    std::println("Final price (cents): {}", total2);
}
