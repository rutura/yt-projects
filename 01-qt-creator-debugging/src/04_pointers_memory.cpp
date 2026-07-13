#include <memory>
#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 04 - Pointers, memory view & undefined behavior
//
// What to try in Qt Creator:
//   * Set a breakpoint at the start of dangling_pointer_demo() (the
//     `int* leaked_ptr = make_dangling();` line). Step over it with F10
//     and look at `leaked_ptr` in the Locals view: after make_dangling()
//     has already returned, the `int` it pointed to no longer exists, but
//     `leaked_ptr` still holds that (now-invalid) address. This is a
//     "dangling pointer" -- notice the debugger happily shows you the
//     address; it can't tell you the memory behind it is no longer valid.
//   * Set a breakpoint inside buffer_overrun_demo() on the
//     `std::println("buffer sum = {}", raw_sum);` line. Once stopped,
//     right-click `buffer` in the Locals view and choose
//     "Open Memory Editor" (or one of its "Open Memory Editor at ..."
//     variants) to see the array's raw bytes as hex and ASCII side by
//     side, instead of as individual `int` values.
//     Optional deeper exercise: uncomment the line
//     `// buffer[10] = 0xFF;` below, rebuild, set the breakpoint one line
//     later, and reopen the Memory Editor on `buffer` to watch the
//     out-of-bounds write land one slot past the array -- a real,
//     visible buffer overrun.
//   * Set a breakpoint inside smart_pointer_demo() and inspect `owner`
//     (a std::unique_ptr<int>) in Locals: Qt Creator's pretty-printer
//     shows you the pointee's value (123) directly, instead of the raw
//     internal pointer bytes you'd see for a hand-rolled smart pointer.
// ---------------------------------------------------------------------------

namespace {

// Deliberately returns the address of a local: this IS the bug the
// scenario teaches. Your compiler will likely warn about this
// (-Wreturn-local-addr / C4172) -- that warning is itself part of the
// lesson: the compiler is telling you the same thing the debugger will
// show you at runtime.
int* make_dangling() {
    int local_value = 77;
    return &local_value; // returning address of a local: dangling on return
}

void dangling_pointer_demo() {
    int* leaked_ptr = make_dangling(); // <-- breakpoint here
    std::println("leaked_ptr address = {}", static_cast<const void*>(leaked_ptr));
    // Deliberately NOT dereferencing leaked_ptr here (that would be UB in
    // the *host* program). Use the debugger's Expressions window to safely
    // *inspect* it instead of letting the program itself dereference it.
}

void buffer_overrun_demo() {
    int buffer[10];
    for (int i = 0; i < 10; ++i) {
        buffer[i] = i * i;
    }

    // Off-by-one: index 10 is one past the end of a 10-element array.
    // Left in intentionally so students can see the debugger's Memory
    // view catch a write that lands just past `buffer`.
    // buffer[10] = 0xFF; // <-- uncomment to trigger the overrun; see README

    int raw_sum = 0;
    for (int i = 0; i < 10; ++i) {
        raw_sum += buffer[i];
    }
    std::println("buffer sum = {}", raw_sum);
}

void smart_pointer_demo() {
    std::unique_ptr<int> owner = std::make_unique<int>(123);
    // <-- breakpoint: inspect `owner` in Locals
    std::println("owner points to {}", *owner);

    int* raw = owner.get();
    std::println("raw alias = {}", *raw);
}

} // namespace

void scenarios::run_pointers_memory() {
    std::println("-- dangling pointer demo --");
    dangling_pointer_demo();

    std::println("-- buffer bounds demo --");
    buffer_overrun_demo();

    std::println("-- smart pointer demo --");
    smart_pointer_demo();
}
