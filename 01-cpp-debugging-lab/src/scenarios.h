#pragma once

// Each scenario is a self-contained "chapter" of the debugging lab.
// main.cpp presents a menu and calls straight into one of these.
// See README.md for what to inspect in your debugger for each one.

namespace scenarios {

void run_breakpoints_stepping();      // 01
void run_variables_watches();         // 02
void run_call_stack_recursion();      // 03
void run_conditional_breakpoints();   // 04
void run_pointers_memory();           // 05
void run_containers_stl();            // 06
void run_exceptions();                // 07
void run_templates_generic();         // 08
void run_multithreading();            // 09

} // namespace scenarios
