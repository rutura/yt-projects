#include <format>
#include <print>
#include <string>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 02 - Locals, Expressions & changing values live
//
// What to try in Qt Creator:
//   * Set a breakpoint on the `std::println("checkpoint A");` line (click
//     the left margin, or F9 with the cursor on that line).
//   * Start debugging (F5). When it stops, the Locals view should already
//     be visible at the bottom-left of the Debug mode layout. If you
//     don't see it (or the Expressions view next to it), go to
//     View > Views and make sure "Locals" and "Expressions" are checked.
//   * Expand `player` in the Locals view (click the triangle/arrow next
//     to it) to see its members: name, health, mana, position_x,
//     position_y.
//   * Right-click `inventory` in Locals and choose
//     "Add Expression Evaluator for 'inventory'" -- this pins it to the
//     Expressions view so you can keep an eye on it even after it
//     scrolls out of Locals. Alternatively, double-click empty space in
//     the Expressions view and type `inventory.size()` directly.
//   * Add another expression evaluator for `player.health < 30` to watch
//     it evaluate live, as a boolean, while you step.
//   * Change `player.health` at runtime: in the Locals view, click
//     directly on the VALUE of `health` (not the name), type `5`, and
//     press Enter. Then step forward (F10) past the second
//     `const bool is_critical_now = ...` line and watch it actually
//     evaluate to true, because you changed the underlying data.
// ---------------------------------------------------------------------------

namespace {

struct Player {
    std::string name;
    int health;
    int mana;
    float position_x;
    float position_y;
};

std::string status_line(const Player& p, bool is_critical) {
    return std::format(
        "{} @ ({:.1f},{:.1f}) hp={} mana={} critical={}",
        p.name, p.position_x, p.position_y, p.health, p.mana, is_critical);
}

} // namespace

void scenarios::run_variables_watches() {
    Player player{
        .name = "Aria",
        .health = 42,
        .mana = 15,
        .position_x = 3.5f,
        .position_y = -1.25f,
    };

    std::vector<std::string> inventory{"sword", "shield", "potion", "map"};

    std::println("checkpoint A"); // <-- set breakpoint here

    const bool is_critical = player.health < 30;
    std::println("{}", status_line(player, is_critical));

    // Mutate state so re-inspecting Locals after stepping shows a change.
    player.mana -= 5;
    inventory.push_back("scroll");

    std::println("checkpoint B"); // <-- set a second breakpoint here

    const bool is_critical_now = player.health < 30;
    std::println("{}", status_line(player, is_critical_now));
    std::println("Inventory has {} items", inventory.size());
}
