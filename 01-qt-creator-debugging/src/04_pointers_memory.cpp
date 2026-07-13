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
//     "dangling pointer" -- the debugger can't tell you the memory
//     behind it is no longer valid, so it just shows you whatever is
//     there.
//     By default, Qt Creator's "Dereference Pointers Automatically"
//     setting is on, so Locals shows `*leaked_ptr` directly with its
//     pointed-to value (77 here) instead of the raw address -- the
//     pointer row itself is collapsed into its dereferenced value. If
//     you right-click `leaked_ptr` and turn that option off, Locals
//     instead shows two rows: `leaked_ptr` with its raw hex address and
//     type `int *`, and a nested `*leaked_ptr` row underneath showing
//     the same value (77) it points to. Either way, that value being
//     readable at all is the "seems to work by accident" trap: the
//     stack memory for `local_value` hasn't been overwritten by
//     anything else yet, so the debugger can still read 77 out of it,
//     even though that memory is no longer valid for your program to
//     use.
//   * Set a breakpoint inside buffer_overrun_demo() on the
//     `std::println("buffer sum = {}", raw_sum);` line. Once stopped,
//     right-click `buffer` in the Locals view and choose
//     "Open Memory Editor" (or one of its "Open Memory Editor at ..."
//     variants). A new window titled something like
//     'Memory at Object's Address "buffer" (0x...)' opens. Making sense
//     of it:
//     - The leftmost column (e.g. "0000:00ac:585c:fb50") is just the
//       memory address of the start of that row -- it climbs by 0x10
//       (16 bytes) per row, since each row shows 16 bytes.
//     - The middle block is 16 individual bytes per row, in hex, two
//       hex digits each (00-ff).
//     - The rightmost block is the exact same 16 bytes, but shown as
//       ASCII -- most of them render as "." because raw int bytes
//       usually aren't printable characters.
//     - `buffer` is `int[10]`, and each `int` is 4 bytes, stored
//       little-endian (least-significant byte first). So the 4 bytes
//       `04 00 00 00` are the int value 4, and `51 00 00 00` are the
//       int value 0x51 = 81 -- these are buffer[2] and buffer[9],
//       matching `i * i` for i=2 and i=9. The Memory Editor usually
//       color-highlights the address range belonging to `buffer` (here,
//       10 groups of 4 bytes) so you can see exactly where the array
//       starts and ends versus the unrelated stack bytes around it.
//     Optional deeper exercise: uncomment the line
//     `// buffer[10] = 0xFF;` below, rebuild, set the breakpoint one line
//     later, and reopen the Memory Editor on `buffer`. Compare the same
//     highlighted region: you'll now see an extra `ff 00 00 00` sitting
//     immediately after the highlighted range for buffer[9] -- a write
//     that landed one slot past the array, in memory `buffer` doesn't
//     own. That's the out-of-bounds write made visible, byte for byte.
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
