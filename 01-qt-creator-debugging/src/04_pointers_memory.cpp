#include <memory>
#include <print>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 04 - Pointers, memory view & undefined behavior
//
// What to try in Qt Creator:
//   * Set a breakpoint at the start of dangling_pointer_demo(). Step through
//     with F10 and watch `leaked_ptr` in Locals: after make_dangling()
//     returns, the pointee is destroyed but the pointer still holds the
//     (now invalid) address. Try dereferencing it in the Expressions view
//     to see how the debugger reacts to reading freed memory.
//   * Set a breakpoint at `buffer[10] = 0xFF;` inside buffer_overrun_demo().
//     Open View > Debugger Windows > Memory > "Open Memory Editor" on
//     `buffer` and watch the out-of-bounds write land in adjacent stack
//     memory. This is a classic case where the program "seems to work"
//     but corrupts something nearby.
//   * Right-click a pointer variable (e.g. `raw`) in Locals and choose
//     "Open Memory Editor" to see the raw bytes it points to, and toggle
//     between hex/ASCII.
//   * Inspect `owner` (a unique_ptr) in Locals: Qt Creator's pretty-printer
//     shows the pointee directly instead of raw internals.
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
