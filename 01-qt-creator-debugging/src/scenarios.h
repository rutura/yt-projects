#pragma once

// Each scenario is a self-contained "chapter" of the debugging lab.
// main.cpp presents a menu and calls straight into one of these.
// See README.md for what to inspect in Qt Creator for each one.

namespace scenarios {

void run_breakpoints_stepping();      // 01
void run_variables_watches();         // 02
void run_call_stack_recursion();      // 03
void run_pointers_memory();           // 04
void run_containers_stl();            // 05
void run_exceptions();                // 06
void run_multithreading();            // 07
void run_templates_generic();         // 08
void run_conditional_breakpoints();   // 09
void run_optimized_release_debugging(); // 10

} // namespace scenarios
