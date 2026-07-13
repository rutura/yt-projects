#include <cstdlib>
#include <iostream>
#include <limits>
#include <print>
#include <string>

#include "scenarios.h"

namespace {

struct MenuItem {
    int number;
    const char* title;
    const char* file;
    void (*run)();
};

const MenuItem kMenu[] = {
    {
        1,
        "Breakpoints & stepping (step in/over/out, run to line)",
        "01_breakpoints_stepping.cpp",
        scenarios::run_breakpoints_stepping,
    },
    {
        2,
        "Locals, watches & Expressions view",
        "02_variables_watches.cpp",
        scenarios::run_variables_watches,
    },
    {
        3,
        "Call stack & recursion",
        "03_call_stack_recursion.cpp",
        scenarios::run_call_stack_recursion,
    },
    {
        4,
        "Pointers, memory view & undefined behavior",
        "04_pointers_memory.cpp",
        scenarios::run_pointers_memory,
    },
    {
        5,
        "STL containers & pretty-printers",
        "05_containers_stl.cpp",
        scenarios::run_containers_stl,
    },
    {
        6,
        "Exceptions & the exception breakpoint",
        "06_exceptions.cpp",
        scenarios::run_exceptions,
    },
    {
        7,
        "Multithreading (Threads view, data races, deadlocks)",
        "07_multithreading.cpp",
        scenarios::run_multithreading,
    },
    {
        8,
        "Templates & generic code (instantiation quirks)",
        "08_templates_generic.cpp",
        scenarios::run_templates_generic,
    },
    {
        9,
        "Conditional / logging breakpoints in a hot loop",
        "09_conditional_breakpoints.cpp",
        scenarios::run_conditional_breakpoints,
    },
    {
        10,
        "Debug vs. Release: optimized-build debugging quirks",
        "10_optimized_release_debugging.cpp",
        scenarios::run_optimized_release_debugging,
    },
};

void print_menu() {
    std::println("==================================================================");
    std::println(" Qt Creator C++23 Debugging Lab");
    std::println("==================================================================");
    for (const auto& item : kMenu) {
        std::println("  [{:2}] {:<55} ({})", item.number, item.title, item.file);
    }
    std::println("  [ 0] Quit");
    std::println("------------------------------------------------------------------");
    std::print("Pick a scenario number and press Enter: ");
}

} // namespace

int main() {
    while (true) {
        print_menu();

        int choice = -1;
        if (!(std::cin >> choice)) {
            if (std::cin.eof()) {
                // Input stream closed (e.g. redirected/non-interactive
                // stdin) -- nothing more will ever arrive, so stop
                // instead of looping forever.
                std::println("\nInput closed. Bye!");
                break;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::println("\nPlease enter a number.\n");
            continue;
        }

        if (choice == 0) {
            std::println("Bye!");
            break;
        }

        bool found = false;
        for (const auto& item : kMenu) {
            if (item.number == choice) {
                found = true;
                std::println("\n--- Running scenario {}: {} ---\n",
                              item.number, item.title);
                item.run();
                std::println("\n--- Scenario {} finished ---\n", item.number);
                break;
            }
        }

        if (!found) {
            std::println("\nUnknown scenario '{}'.\n", choice);
        }
    }

    return EXIT_SUCCESS;
}
