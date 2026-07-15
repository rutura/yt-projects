# C++23 Debugging Lab

A single CMake project built to give you hands-on practice with every major
feature of a modern C++ debugger, using plain, modern C++23 (no Qt libraries
required). Each numbered scenario is a small, self-contained "chapter" that
puts you in front of one debugging concept: breakpoints, inspecting and
changing variables, the call stack, conditional breakpoints, memory
inspection, exceptions, templates and threads.

**This guide assumes you have never used a debugger before.** By the end of
all nine scenarios, you should be comfortable enough with your debugger to
reach for it by default instead of adding `std::println` statements
everywhere to figure out what your program is doing.

The instructions below describe debugger *concepts* and their common
keyboard shortcuts (F5/F9/F10/F11), which match the default keymap in most
mainstream C++ IDEs (Qt Creator, Visual Studio, CLion, VS Code with the
C/C++ extension). The underlying view names — Locals, Watch/Expressions,
Call Stack, Breakpoints, Memory, Threads — are effectively universal across
debugger front ends (GDB, LLDB, CDB); only their exact menu location and
occasional shortcut differ between IDEs. If a shortcut doesn't match your
setup, check your IDE's keyboard shortcuts reference — the *concept* behind
each step still applies everywhere.

---

## 1. Prerequisites

- Any IDE or editor with an integrated C++ debugger (Qt Creator, Visual
  Studio, CLion, VS Code, etc.), or a standalone debugger (GDB, LLDB, CDB)
  driven from the command line.
- A C++23-capable compiler. This project uses `<print>`/`<format>`, which
  needs a genuinely recent standard library, not just a compiler that
  claims `-std=c++23`:
  - MSVC 2022 17.5+.
  - GCC 14+ or Clang 17+ (Linux, macOS, or MinGW/LLVM-MinGW on Windows).
    Older toolchains (e.g. GCC 13) predate `<print>`/`<format>` support and
    will fail to compile.
- A matching debugger backend:
  - MSVC toolchains typically use **CDB** (Windows Debugging Tools).
  - GCC/Clang toolchains use **GDB** or **LLDB**.
  - A few things in this guide differ slightly depending on which backend
    you have
- CMake 3.25+.

No third-party libraries are used — only the standard library and
`std::thread`/`Threads::Threads` for the multithreading scenario.

---

## 2. Opening the project

1. Configure and build the project with CMake, either through your IDE's
   built-in CMake integration (open the folder / open `CMakeLists.txt`) 
2. The only build target is `cpp_debugging_lab`.

Running or debugging the program opens a console window
showing:

```
==================================================================
 C++23 Debugging Lab
==================================================================
  [ 1] Breakpoints & stepping (step in/over/out, run to line)  (01_breakpoints_stepping.cpp)
  [ 2] Locals, watches & Expressions view                      (02_variables_watches.cpp)
  ...
  [ 0] Quit
------------------------------------------------------------------
Pick a scenario number and press Enter:
```

Type a number and press Enter to run that scenario. **Set your breakpoints
in the relevant source file first** (before you start debugging), then
start debugging and type the scenario's number at the prompt.

> If the program appears to hang, or repeatedly prints "Please enter a
> number." / "Input closed." without ever letting you type, that almost
> always means the run configuration isn't launching in a real terminal.
> Stop the program and fix that setting first.

---

## 3. Your first debugging session, explained slowly

If you've genuinely never used an IDE debugger before, read this section
before jumping into Scenario 1 — it explains *why* each action matters, not
just *how* to trigger it.

**What a debugger actually does.** Normally your program just runs start to
finish. A debugger lets you freeze it at an exact line of code, look at
every variable's current value, and then move forward one step (or one
line, or one function) at a time, watching values change as you go. This
is dramatically faster than the alternative of sprinkling `std::println`
calls everywhere and rebuilding after every guess.

**Breakpoints.** A breakpoint marks a line where you want execution to
pause. In most IDEs you set one by clicking in the thin margin to the left
of the line numbers, or by putting your text cursor on that line and
pressing **F9**. A marker (usually a red dot) appears. This works whether
or not you're currently debugging — you can set breakpoints ahead of time,
before you ever start a debug session.

**Starting a debug session.** Press **F5** (or your IDE's "Start
Debugging" action). The IDE builds the project if anything changed,
launches it, and lets it run freely until it hits one of your breakpoints
(or you interrupt it manually, or it crashes, or it finishes). When a
breakpoint is hit, everything pauses: the program is frozen mid-execution,
and the IDE switches into its debugging layout, showing you a collection
of panes with information about exactly where you are.

**The panes you'll use constantly:**
- **Locals** — every variable currently in scope, with its live value.
  Struct and class members can usually be expanded by clicking a small
  arrow/triangle next to them.
- **Call Stack** (sometimes just "Stack") — the chain of function calls
  that got you to where you're currently stopped. The top entry is where
  you are right now; each entry below it is "who called this."

If a pane you need isn't visible, check your IDE's View/Windows menu for
debugger-specific panels (these are usually only listed while a debug
session is active). Every scenario below tells you exactly which pane to
look at.

**Moving forward.** Once paused, you have four core moves, all available
as toolbar buttons or keyboard shortcuts:

| Action | Shortcut | What it does |
|---|---|---|
| Continue | F5 | Resume running at full speed until the next breakpoint (or the program ends). |
| Step Over | F10 | Run just the current line. If it's a function call, the whole call happens, but you don't watch its insides — you land on the next line of the function you're already in. |
| Step Into | F11 | If the current line is a function call, jump inside that function and pause at its very first line. |
| Step Out | Shift+F11 | Run the rest of the function you're currently inside, then pause right after control returns to whoever called it. |
| Stop | Shift+F5 | End the debug session and terminate the program. |

There's also **Run to Line**, usually **Ctrl+F10**: right-click any line in
the editor (while a debug session is active) and choose it, or place your
cursor on the line and use the shortcut. This runs at full speed until
execution reaches that specific line — a one-time breakpoint you don't have
to set up or clean up afterward.

That's most of what you need for Scenario 1. Everything after
that scenario builds on these same five actions.

---

## 4. Scenarios

### Scenario 1 — Breakpoints & stepping
**File:** `src/01_breakpoints_stepping.cpp`

1. Open the file and click in the editor's left margin next to the line
   `double price = base_price(item);` inside `compute_receipt()` (or put
   your cursor there and press F9). A breakpoint marker appears.
2. Start debugging: press F5. Type `1` at the console prompt to run this
   scenario. Execution pauses at your breakpoint.
3. Press **F10 (Step Over)** repeatedly to move through
   `apply_discount()` and `add_tax()` one call at a time, without diving
   into either of them.
4. Stop the session (Shift+F5) and start again (F5, then `1`). This time,
   when you're stopped at the same breakpoint, press F10 twice to reach
   the `double final_price = add_tax(discounted, tax_rate);` line, then
   press **F11 (Step Into)** on that call to actually step inside
   `add_tax()`. Once inside it, press **Shift+F11 (Step Out)** to run the
   rest of `add_tax()` and pop back up to `compute_receipt()`, now with
   `final_price` populated.

**What to notice:** Step Over treats a called function as a black box;
Step Into follows execution inside it; Step Out finishes the current
function and returns to its caller. This is the single most important
muscle-memory skill in the whole lab — everything else assumes you're
comfortable choosing between these three.

---

### Scenario 2 — Locals, watches & changing values live
**File:** `src/02_variables_watches.cpp`

1. Set a breakpoint on `std::println("checkpoint A");`.
2. Debug (F5), choose `2`. When it stops, you should already see the
   **Locals** and **Watch/Expressions** panes (typically bottom-left of
   the debugging layout). If not, check your IDE's debugger view menu and
   enable both.
3. Expand `player` in Locals by clicking its arrow — it's a struct, so
   you'll see each member: `name`, `health`, `mana`, `position_x`,
   `position_y`.
4. Pin `inventory` to your Watch/Expressions view — most IDEs offer this
   via a right-click "Add Watch/Expression" option on the variable in
   Locals, or by typing the expression directly into the Watch panel. This
   keeps it visible even after it scrolls out of Locals.
5. The same way, add a watch for `player.health`. Step forward (F10) and
   watch it stay live-evaluated as you go.

   > Stick to expressions like a bare variable, a member access
   > (`player.health`), or a function call (`inventory.size()`) — these
   > evaluate reliably on every debugger backend (GDB, LLDB, CDB).
   > Comparison/boolean expressions typed into a watch (for example
   > `player.health < 30`) are known to be unreliable specifically under
   > LLDB, and can show something like `<no such value>` even though every
   > piece of the expression is genuinely in scope. That's a limitation of
   > the debugger backend's expression parser, not a bug in your program —
   > if you need to check a condition like that, just step to the line
   > that computes it (see step 6) and read the resulting variable in
   > Locals instead.
6. Change a value at runtime: in the Locals view, click directly on the
   **value** of `health` (not its name), type `5`, and press Enter. Then
   step forward (F10) past the second
   `const bool is_critical_now = player.health < 30;` line and check
   `is_critical_now` in Locals — it now evaluates to `true`, because you
   changed the underlying data yourself, live, without editing or
   recompiling any code.

**What to notice:** Locals shows everything currently in scope
automatically. A watch/expressions view lets you pin specific values
(including member accesses and function calls, not just plain variables)
so you can track them across the whole debug session, even as you step
through unrelated code. Editing a value directly in Locals is a fast way
to test "what if" branches without touching source code at all.

---

### Scenario 3 — Call stack & recursion
**File:** `src/03_call_stack_recursion.cpp`

1. Set a breakpoint inside `fibonacci()` on the
   `return fibonacci(n - 1) + fibonacci(n - 2);` line.
2. Debug (F5), choose `3`. Make sure the **Call Stack** view is visible.
3. Press F5 (Continue) a few times and watch the Call Stack view: it grows
   by one entry every time `fibonacci()` calls itself, because each call
   is a brand-new, still-unfinished frame stacked on top of the last one.
   Click different entries in the Call Stack view — the Locals view
   updates to show `n`'s value at that specific level of recursion.
4. Instead of Continue, try **Shift+F11 (Step Out)** repeatedly: each
   press finishes the innermost still-running `fibonacci()` call and pops
   you back one level, and you can watch the Call Stack view shrink.
5. Stop and restart, this time setting a breakpoint inside
   `level_three()`, then debug and choose `3` again. This call chain
   (`deeply_nested_call` → `level_one` → `level_two` → `level_three`)
   isn't recursive — it's four distinctly named functions calling into
   each other, which is closer to what a real, non-recursive call stack
   looks like.

**What to notice:** The Call Stack view is your map of "how did I get
here." Recursive code can get deep fast — that's intentional here, so you
get comfortable navigating many stacked frames instead of just one or two.

---

### Scenario 4 — Conditional breakpoints
**File:** `src/04_conditional_breakpoints.cpp`

We want a breakpoint that only triggers when a specific condition is met,
instead of stopping on every hit. Below, the same line runs once per
order, with a different `quantity` and `unit_price` each time. A plain
breakpoint there stops on every order; a conditional breakpoint lets you
stop only on the one you actually care about.

1. Set a breakpoint on the `const int total = quantity * unit_price;`
   line.
2. Debug (F5), choose `4`. Notice it stops on every single order.
3. Stop the session. Right-click that breakpoint (its marker in the
   editor margin, or its row in the Breakpoints view) and find its edit/
   properties option. Look for a **Condition** field and enter
   `quantity > 10`. Debug again — the debugger skips straight to the one
   order where that's true, instead of stopping on all of them.
4. Try a different condition instead, such as `unit_price == 0`, to land
   on the free/comped order.

**What to notice:** Condition is one field on the same breakpoint
properties dialog you'll come back to for logging breakpoints and ignore
counts later — the underlying idea is always "stop here, but only when
this is true," instead of forcing you to manually skip past every
uninteresting hit.

---

### Scenario 5 — Pointers, memory view & undefined behavior
**File:** `src/05_pointers_memory.cpp`

1. Set a breakpoint on `int* leaked_ptr = make_dangling();` inside
   `dangling_pointer_demo()`.
2. Debug (F5), choose `5`. Step over it (F10) and look at `leaked_ptr` in
   Locals — it points to the `int` that used to be `local_value` inside
   `make_dangling()`, which was destroyed the instant that function
   returned. This is a **dangling pointer**: the debugger has no way of
   knowing the memory behind it is no longer valid, so it just shows you
   whatever is still sitting there.
   - Many debugger front ends automatically dereference pointers in
     Locals, so you may see a single row, `*leaked_ptr`, with the
     dereferenced value (`77`) shown directly. If your IDE shows the raw
     pointer instead, you'll see `leaked_ptr` with its hex address and
     type `int *`, plus a way to expand it into a `*leaked_ptr` row with
     the same value (`77`) underneath.
   - Either way, the fact that `77` is still readable at all is the
     trap: the stack memory for `local_value` hasn't been overwritten by
     anything else yet, so the debugger can still read the old value out
     of it — even though that memory is no longer valid for your program
     to use. This is exactly why dangling-pointer bugs are dangerous:
     they often *appear* to work by pure accident.
   - (You may have already noticed your compiler warned about this at
     build time — `C4172` on MSVC/CDB, `-Wreturn-local-addr` or
     `-Wreturn-stack-address` on GCC/Clang. That's the same bug, caught
     two different ways: once by the compiler while reading your source,
     and once by the debugger while your program actually runs.)
3. Set a breakpoint inside `buffer_overrun_demo()`, on the
   `std::println("buffer sum = {}", raw_sum);` line. Step to it, then find
   your IDE's memory inspection view for `buffer` (often reached via a
   right-click "Open Memory Editor"/"View Memory" option, or a dedicated
   Memory panel where you can type the variable's address). Here's how to
   read it:
   - The leftmost column is just the memory address where that row
     starts. It typically climbs by `0x10` (16 bytes) per row, since each
     row displays 16 bytes.
   - The middle block is those same 16 bytes, one at a time, in hex (two
     digits each, `00`–`ff`).
   - The rightmost block is the identical 16 bytes shown as ASCII text.
     Most will render as `.`, because raw `int` bytes usually aren't
     printable characters.
   - `buffer` is `int[10]`, and each `int` occupies 4 bytes, stored
     **little-endian** (least-significant byte first). So the 4 bytes
     `04 00 00 00` are the value `4`, and `51 00 00 00` are `0x51 = 81`
     in decimal — those are `buffer[2]` and `buffer[9]`, matching
     `i * i` for `i = 2` and `i = 9`. Many memory views highlight the
     exact address range belonging to `buffer` (10 groups of 4 bytes
     here), so you can see precisely where the array starts and ends
     versus the unrelated stack bytes surrounding it.
   - Optional deeper exercise: uncomment the line
     `// buffer[10] = 0xFF;` in the source, rebuild, move your breakpoint
     one line later, and reopen the memory view on `buffer`. Compare the
     same highlighted region: you'll now see an extra `ff 00 00 00`
     sitting immediately after the bytes for `buffer[9]` — a write that
     landed one slot past the array, in memory `buffer` doesn't own.
     That's the off-by-one overrun made visible, byte for byte.
4. Set a breakpoint on `std::println("owner points to {}", *owner);`
   inside `smart_pointer_demo()`. Inspect `owner` (a
   `std::unique_ptr<int>`) in Locals — most debugger backends' pretty-
   printers show the pointee's value (123) directly, instead of raw
   internal pointer bytes.

**What to notice:** The debugger lets you *safely inspect* values that
would be dangerous to actually use in code (like dereferencing
`leaked_ptr` yourself). A memory view is how you see what's really in
memory, byte for byte, which matters once "just look at Locals" stops
being enough to explain a bug.

---

### Scenario 6 — STL containers & pretty-printers
**File:** `src/06_containers_stl.cpp`

1. Set a breakpoint on the `std::optional<int> maybe_missing = 42;` line
   (by this point, `scores`, `name_to_id`, and `fast_lookup` are already
   filled in, so all three are visible in Locals when you stop here).
2. Debug (F5), choose `6`. Expand `scores` (a `std::vector<int>`) in
   Locals: instead of raw internal pointers, you get a clean, indexed
   list of the actual `int` values — this readable rendering is called a
   "pretty-printer."
3. Expand `name_to_id` (a `std::map<std::string,int>`) and `fast_lookup`
   (a `std::unordered_map<std::string,int>`) the same way, and compare
   how each renders its key/value pairs.
4. Step over (F10) to the `maybe_missing.reset();` line. Look at
   `maybe_missing` (a `std::optional<int>`) in Locals before you step
   over that line, then again right after — the pretty-printer shows a
   clear "has a value" vs. "empty" state, and you watch it flip live.
5. Set a breakpoint inside the `for (int s : high_scores)` loop, on the
   `std::print("{} ", s);` line. Step through it (F10) one iteration at a
   time. `high_scores` is a `std::ranges` pipeline (filter + transform):
   unlike `scores`, it isn't a real container sitting fully in memory —
   it computes each value lazily as you iterate, which is exactly what
   you're watching happen one value at a time.

**What to notice:** Your debugger relies on its backend's (GDB, LLDB, or
CDB's) pretty-printers to turn raw container internals into readable
views. This is why modern C++ containers are comfortable to debug — you
almost never need to manually decode a vector's internal
pointer/size/capacity triplet by hand.

---

### Scenario 7 — Exceptions & breaking on the throw site
**File:** `src/07_exceptions.cpp`

The setup step here depends on which debugger backend you're using.

**If you're on CDB (typical with MSVC):**
1. Find your IDE's debugger preferences for CDB and locate a "Break On"
   (or similarly named) group.
2. Enable its option for C++ exceptions.
3. This is a standing preference, not a per-breakpoint setting — once
   enabled, it applies to every debug session until you turn it off
   again.

**If you're on GDB or LLDB (typical for MinGW/LLVM-MinGW/Linux/macOS):**
1. Open your IDE's Breakpoints view.
2. Add a new breakpoint, but set its type to break on a thrown C++
   exception (rather than a source line) — most IDEs expose this as an
   option in the "Add Breakpoint" dialog.

Either way, once exception breaking is enabled:

1. Start debugging (F5), choose `7`. Execution stops the instant
   `throw std::invalid_argument(...)` runs inside `divide()` — **not** at
   the `catch` block — so you land exactly where and why it was thrown,
   with the full call stack back to wherever `divide()` was called from.
2. Continue (F5). Now set a normal breakpoint inside the `catch` block
   (the `std::println("Caught expected exception: {}", ...)` line) and
   inspect the caught object `e` in Locals — expand it to see the message
   stored inside, the same text `.what()` returns.
3. Turn exception breaking back off and debug again. This time execution
   jumps straight to the `catch` block with no pause at the throw site —
   you've lost visibility into exactly where the exception came from,
   which is precisely why exception breakpoints matter once you're
   debugging a codebase you didn't write from scratch.

**What to notice:** By default, a debugger only stops you at breakpoints
you've explicitly set, or when the program crashes — not at every
`throw`. Breaking on exceptions is how you catch the *moment* something
goes wrong, instead of only where it's eventually handled (or, in a real
bug, not handled at all).

---

### Scenario 8 — Templates & generic code (C++23)
**File:** `src/08_templates_generic.cpp`

**This scenario assumes you have never worked with templates before**,
so before touching the debugger, here's what's actually happening in
the code.

#### What is a template?

Every function you've debugged so far in this lab was compiled exactly
once — its source and its compiled code are basically the same thing. A
**template** breaks that assumption: it's not compiled code at all, it's
a *blueprint* that only becomes real, compiled code once you actually
use it with a specific type. `sum_all<T>` in this file is declared once,
but `run_templates_generic()` calls it twice, with two different types:

```
 template sum_all<T>   (blueprint, not yet real code)
    |
    |-- called with vector<int>    --> compiler stamps out sum_all<int>
    |-- called with vector<double> --> compiler stamps out sum_all<double>
```

Each of those stamped-out versions is called an **instantiation** — a
genuinely separate, independently compiled function, even though both
came from the one block of source you wrote. This is why, later in the
debugger, a breakpoint on one single source line can report two
different function names depending on which call reached it.

`Box<T>` works the same way for a class template: `Box<std::string>` in
this file causes the compiler to generate a real `Box` type specialized
for `std::string`, with its own compiled `describe()` method — nothing
about `Box<T>` exists as compiled code until some concrete `T` is used.

#### `concept` — constraining what a template will accept

```cpp
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <Numeric T>
T sum_all(const std::vector<T>& values) { ... }
```

A **concept** is a named, compiler-checkable rule about what a template
argument is allowed to be. `Numeric` says "an integer type, or a
floating-point type." Writing `template <Numeric T>` instead of
`template <typename T>` tells the compiler to reject anything that
doesn't satisfy `Numeric` immediately, with a clear error pointing at
the call site — instead of accepting the call, generating an
instantiation, and only failing later, deep inside `sum_all`'s body,
with a much more confusing error. A concept doesn't create more
instantiations or change what gets compiled; it only *gatekeeps* which
types are allowed to trigger an instantiation in the first place.

#### `if consteval` — one function, two different bodies

```cpp
constexpr int classify(int n) {
    if consteval {
        return n < 0 ? -1 : 1;          // compile-time-only body
    } else {
        return n < 0 ? -1 : (n == 0 ? 0 : 1); // runtime body
    }
}
```

This is a different kind of "two versions from one function" than
templates: instead of branching on *type*, `if consteval` branches on
*when* the call happens. If the compiler can fully evaluate the call
during compilation (a **compile-time context**), it runs the first
branch — and that branch's code never exists as instructions in your
final program; it ran once, while the compiler was compiling, and its
result was baked directly into the binary. If the call happens while
your program is actually running (a **runtime context**), the second
branch runs instead, as ordinary compiled code:

```
classify(n) called
    |
    |-- in a compile-time context --> "if consteval" branch runs
    |                                 *during compilation*; no runtime
    |                                 instructions left to debug
    |
    |-- in a runtime context      --> "else" branch runs as normal
                                       compiled code; this is the only
                                       branch a live debugger can ever
                                       stop inside
```

`run_templates_generic()` calls `classify(runtime_value)` where
`runtime_value` isn't a compile-time constant, so it always takes the
runtime (`else`) path in this scenario.

#### Now, in the debugger

1. Set a breakpoint inside `sum_all()` on the `total += v;` line.
2. Debug (F5), choose `8`. When it stops (from the `sum_all(ints)` call),
   check the **Call Stack** view: the top frame's name isn't just
   "sum_all", it shows the concrete instantiated type, something like
   `sum_all<int>` — direct confirmation that the blueprint diagram above
   is what actually happened at compile time.
3. Continue (F5): the same breakpoint fires again, this time from the
   `sum_all(doubles)` call. Check the Call Stack view again — now it
   shows `sum_all<double>`. Same source line, same breakpoint, two
   genuinely different compiled functions underneath.
4. Set a breakpoint inside `Box<T>::describe()`, on the
   `return std::format(...)` line. When it stops, expand `this` in
   Locals — it's a pointer to the current `Box` object, and expanding it
   shows its `value` member, the same way expanding any other pointer
   would. Note the Call Stack view's frame name here too — it will show
   the instantiated type, `Box<std::string>::describe`, not just `Box`.
5. Set a breakpoint inside `classify()`, specifically on the
   `return n < 0 ? -1 : (n == 0 ? 0 : 1);` line (the one inside the
   `else`). Step through it — this is the runtime branch, so it behaves
   like any other line you've debugged so far. Now try setting a
   breakpoint on the line *above* it, inside the `if consteval` block
   (`return n < 0 ? -1 : 1;`). Debug again: that breakpoint never fires.
   That's not a bug — it's the "if consteval" diagram above made literal.
   Since `classify()` is only ever called here with a runtime value,
   that branch is dead for this program's *runtime*, even though the
   compiler still had to understand it while compiling.

**What to notice:** Templates are compiled per instantiation — the
debugger always shows you the concrete, generated function for the types
you actually used, which is why the exact same source line can appear
under different "instantiated" names in the Call Stack view depending on
how it was called. `if consteval`'s compile-time branch is a step
further: it's not just a different compiled function, it's not compiled
into runtime code *at all*, which is why a live debugger can never pause
inside it.

---

### Scenario 9 — Multithreading (threads view, data races)
**File:** `src/09_multithreading.cpp`

**This scenario assumes you have never worked with threads before**, so
before touching the debugger, here's what's actually happening in the
code, and why it can go wrong.

#### What is a thread?

Every program you've debugged so far in this lab has a single thread:
one instruction pointer, moving through your code one step at a time,
start to finish.

```
main() ------------------------------------------------------> end
```

Multithreading means starting several of these "execution pointers" at
once. They all run *simultaneously* (or at least appear to — more on
that below), and, critically, they all share the same memory: the same
global variables, the same heap objects. `race_condition_demo()` in this
file spawns `kThreadCount` (4) worker threads that each independently run
`increment_unsafe()`:

```
 main thread
    |
    |-- spawn --> Thread 1: increment_unsafe() --\
    |-- spawn --> Thread 2: increment_unsafe() ---\
    |-- spawn --> Thread 3: increment_unsafe() ---- all 4 running
    |-- spawn --> Thread 4: increment_unsafe() ---/ at the same time
    |                                             /
    |<---------- t.join() waits for all 4 -------/
    v
 main thread continues, prints the result
```

`t.join()` is how the main thread waits for a worker to finish — without
it, `main` could print the result before the workers are even done.
Each worker runs the *same* function independently, but all four are
incrementing the *one* shared `unsafe_counter` variable declared at
file scope — that sharing is exactly where the trouble starts.

#### Why sharing memory is dangerous: the data race

`++unsafe_counter;` reads like a single, indivisible action in C++
source. It isn't. The CPU actually carries it out in three separate
steps:

```
1. READ  unsafe_counter from memory into a register
2. ADD   1 to that register
3. WRITE the register's new value back to memory
```

As long as only one thread ever does this, that's harmless. But once
two threads interleave these three steps on the *same* memory, an
update can vanish. Walk through what happens if both threads start from
`unsafe_counter == 5`:

```
                time ------------------------------------------->
Thread 1:   READ(5)   ADD->6              WRITE 6
Thread 2:                     READ(5)   ADD->6              WRITE 6
----------------------------------------------------------------------
counter:  5        5           5          6          6         6
```

Both threads read `5` *before either had written back*, so one whole
increment is lost — the counter should now be `7`, but it's `6`. This is
a **data race**: two-or-more threads touching the same memory at the
same time, at least one of them writing, with nothing forcing their
steps to happen one-at-a-time. It's undefined behavior in C++, and with
4 threads each doing 100,000 increments (`kIncrementsPerThread`), plenty
of updates get quietly dropped this way.

#### Two ways to fix it

**`std::atomic` (`increment_atomic()`)** — an atomic integer makes the
whole read-modify-write sequence a single indivisible hardware operation.
No other thread can ever observe it "half done." Fast, no waiting
involved.

**`std::mutex` (`increment_safe()`)** — a mutex ("mutual exclusion") is a
lock. Only one thread may hold it at a time; `std::scoped_lock
lock(counter_mutex);` acquires it and automatically releases it when
`lock` goes out of scope at the end of that loop iteration. Every other
thread that wants the lock simply **blocks** (waits) until it's free:

```
Thread 1:  [lock]--[++mutex_counter]--[unlock]
Thread 2:      ...blocked, waiting...          [lock]--[++mutex_counter]--[unlock]
```

This is slower than atomic (idle threads instead of working ones), but a
mutex can protect an entire block of code or several variables at once —
something a single atomic can't do.

#### Now, in the debugger

Don't try to catch the threads by manually pausing the program — 4
threads racing through 100,000 tight increments each finish in well
under a millisecond, far faster than any human can react to. Let a
**breakpoint** do the pausing instead; it's reliable on every run.

1. Make sure your IDE's **Threads** view is visible.
2. Set a breakpoint inside `increment_unsafe()` on the
   `++unsafe_counter;` line.
3. Debug (F5), choose `9`. Execution stops the moment *any* worker
   thread reaches that line. Open the Threads view and look at its
   function/file/line columns:
   - You'll see **more rows than just your 4 workers**. CDB/LLDB also
     list internal OS/runtime threads — rows whose function is
     something like `NtWaitForSingleObject` or
     `NtWaitForWorkViaWorkerFactory`, with no file/line of their own.
     Those aren't your code — ignore them.
   - The rows that matter are the ones whose function name is
     `increment_unsafe` and whose line shows your breakpoint's line
     number. Those are your worker threads. Right after the very first
     stop you'll often see just **1** of these rows, since the other
     workers haven't reached the breakpoint yet.
4. Press **F5 (Continue)** a handful of times (5–10 presses is plenty).
   Each press resumes *every* thread until any one of them hits the
   breakpoint again, so you're sampling the race rather than
   single-stepping it. Re-check the Threads view after each press:
   - Watch the count of `increment_unsafe` rows grow as more workers
     arrive at the breakpoint — you should eventually see up to
     **4 at once**.
   - Watch their **thread ids** — different presses land on different
     threads, direct proof several threads are all racing to reach this
     exact line, over and over, with no fixed order between them.
   - You may also eventually see *fewer* rows than before, if a worker
     finished its entire 100,000-iteration loop and exited between
     presses — that's expected too, not a bug in the setup.
5. Click one `increment_unsafe` row, then another (if you have more than
   one). Watch the thread selector in the debugger toolbar and the
   highlighted row both switch to match — that's your confirmation you
   switched threads. **The Call Stack view will look identical between
   them** — same function names, same line, same depth — and that's
   expected, not a sign nothing happened: `increment_unsafe()` isn't
   recursive, so every worker's call stack always has the same shape.
   What actually differs per-thread is a *value*, not the stack shape:
   open the **Locals** pane and check `i` (the loop counter) while each
   thread is selected — each thread has its own independent copy,
   frozen wherever that particular thread's loop happened to be when it
   hit the breakpoint. This is the debugger making the "spawn" diagram
   above literal: each `increment_unsafe` row really is one of those
   simultaneously-running, independently-progressing arrows.
6. Now **remove the breakpoint** (or disable it). This loop runs 400,000
   total increments — you obviously can't press F5 that many times by
   hand. With the breakpoint gone, press F5 once more to let the program
   run to completion at full speed, and read its own printed output:
   `unsafe_counter = ... (expected ...) -- mismatch shows the race`. The
   two numbers essentially never match, because multiple threads read,
   incremented, and wrote back `unsafe_counter` at the same time without
   any synchronization, silently losing some of those updates — the
   exact scenario walked through in the timing diagram above.
7. Compare against `increment_safe()`, which uses a `std::mutex`. Set a
   breakpoint on the `std::scoped_lock lock(counter_mutex);` line and
   debug again: at any moment, only one thread is ever stopped there —
   check the Threads view and you'll see the rest sitting in a
   waiting/blocked state rather than also being paused at your
   breakpoint, since they're stuck waiting for the mutex to be released.

**What to notice:** `unsafe_counter`'s final value is *non-deterministic*
— run the program a few times and the exact wrong number will vary, but
it will essentially never equal the expected total. That's the signature
of a data race. `safe_counter` (atomic) and `mutex_counter`
(mutex-guarded) are always correct, every time.

---

## 5. Suggested order for a first pass

If you're working through this linearly (e.g. for a video/course), this
order builds skills progressively:

1. Scenario 1 (stepping fundamentals)
2. Scenario 2 (inspecting/changing state)
3. Scenario 3 (reading the call stack)
4. Scenario 4 (conditional breakpoints — once stepping alone gets tedious)
5. Scenario 5 (memory & undefined behavior)
6. Scenario 6 (STL pretty-printers)
7. Scenario 7 (exceptions)
8. Scenario 8 (templates)
9. Scenario 9 (multithreading — most advanced view usage)

## 6. Project layout

```
01-cpp-debugging-lab/
├── CMakeLists.txt                          # C++23, single executable target
├── README.md                               # this file
└── src/
    ├── main.cpp                            # interactive scenario menu
    ├── scenarios.h                         # declares one entry point per scenario
    ├── 01_breakpoints_stepping.cpp
    ├── 02_variables_watches.cpp
    ├── 03_call_stack_recursion.cpp
    ├── 04_conditional_breakpoints.cpp
    ├── 05_pointers_memory.cpp
    ├── 06_containers_stl.cpp
    ├── 07_exceptions.cpp
    ├── 08_templates_generic.cpp
    └── 09_multithreading.cpp
```

Each `.cpp` file has a short header comment pointing back to the matching
section of this README, so once the project is open in your IDE you know
exactly where to look for the full walkthrough.
