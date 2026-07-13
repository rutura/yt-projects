#include <print>
#include <string>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 02 - Locals, Watches & the Expressions view
//
// What to try in Qt Creator:
//   * Set a breakpoint on the `std::println("checkpoint A");` line.
//   * Open View > Debugger Windows > Locals and Expressions.
//   * Watch `player` expand in the Locals view (struct members).
//   * Right-click `inventory` -> "Add to Watch Window" or type
//     `inventory.size()` directly into the Expressions box.
//   * Add a watch expression `player.health < 30` to see it evaluate
//     live as a boolean while you step.
//   * Change `player.health` at runtime: right-click it in Locals ->
//     "Change Value" and set it to 5, then step forward and watch the
//     `is_critical` branch actually take the true path.
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
    return std::format("{} @ ({:.1f},{:.1f}) hp={} mana={} critical={}",
                        p.name, p.position_x, p.position_y, p.health, p.mana, is_critical);
}

} // namespace

void scenarios::run_variables_watches() {
    Player player{.name = "Aria", .health = 42, .mana = 15, .position_x = 3.5f, .position_y = -1.25f};

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
