#include <print>
#include <string_view>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 01 - Breakpoints & stepping
//
// What to try in Qt Creator (see README.md "Scenario 1" for full steps):
//   * Click the gutter next to `int total = base_price(item);` to set a
//     breakpoint, then Debug > Start Debugging (F5).
//   * Use Step Over (F10) to walk through apply_discount() and add_tax()
//     without diving in.
//   * Use Step Into (F11) on compute_receipt() to actually enter it.
//   * Use Step Out (Shift+F11) once inside add_tax() to pop back up.
//   * Try "Run to Line" (Ctrl+F10) targeting the std::println at the bottom.
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
