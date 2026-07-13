# Qt Creator C++23 Debugging Lab

A single CMake project built specifically to give you hands-on practice with
**every major debugging feature in Qt Creator**, using plain, modern C++23
(no Qt libraries required). Each numbered scenario is a small, self-contained
"chapter" that puts you in front of one debugging concept: breakpoints,
watches, the call stack, memory inspection, undefined behavior, STL
pretty-printers, exceptions, threads, templates, conditional breakpoints, and
optimized-build quirks.

Run the program, pick a scenario from the menu, then follow that scenario's
instructions below **before** running it, so your breakpoints are already in
place.

---

## 1. Prerequisites

- **Qt Creator** 12 or newer (Qt Creator ships its own CMake integration and
  debugger front end; you do not need the Qt framework itself for this
  project, only the IDE).
- A C++23-capable compiler, already configured as a **Kit** in Qt Creator:
  - Windows: MSVC 2022 (or newer) via the "Desktop Qt ... MSVC" kit, or
    MinGW-w64 GCC 13+.
  - Linux: GCC 13+ or Clang 16+.
  - macOS: a recent Xcode/Clang toolchain.
- A matching debugger backend, which Qt Creator auto-detects per kit:
  - MSVC kits use **CDB** (Windows Debugging Tools). If Qt Creator warns
    that CDB isn't installed, install "Debugging Tools for Windows" via the
    Windows SDK installer, or switch to a MinGW kit which uses **GDB**.
  - GCC/Clang kits use **GDB** or **LLDB**.
- CMake 3.25+ (Qt Creator can also use its bundled CMake).

No third-party libraries are used — only the standard library and
`std::thread`/`Threads::Threads` for the multithreading scenario.

---

## 2. Opening the project

1. Launch Qt Creator.
2. **File > Open File or Project...** and select `CMakeLists.txt` in this
   folder.
3. Qt Creator will ask you to **configure a Kit** — pick any kit with a
   C++23-capable compiler. Click **Configure Project**.
4. Wait for the CMake configure step to finish (bottom status bar). You
   should see the project tree populate with `src/` and all the numbered
   scenario files.
5. Build once with **Ctrl+B** (or **Build > Build Project**) to confirm
   everything compiles.
6. Set the run target to `qt_creator_debugging_lab` (it's the only target
   for now — see Scenario 10 for adding a second build configuration).

The program is an interactive console menu. When you **Run** it (Ctrl+R) or
**Debug** it (F5), a terminal/Application Output pane opens showing:

```
==================================================================
 Qt Creator C++23 Debugging Lab
==================================================================
  [ 1] Breakpoints & stepping (step in/over/out, run to line)  (01_breakpoints_stepping.cpp)
  [ 2] Locals, watches & Expressions view                      (02_variables_watches.cpp)
  ...
  [ 0] Quit
------------------------------------------------------------------
Pick a scenario number and press Enter:
```

Type a number and press Enter to run that scenario. **Set your breakpoints
in the relevant source file first**, then start debugging and type the
scenario's number at the prompt.

> **Tip:** On Windows with an MSVC kit, make sure Qt Creator's Debugger
> settings run the app in an environment where you can type into stdin —
> the default "Application Output" pane supports input. If you don't see a
> prompt, check **Projects > Run > Run in terminal**.

---

## 3. General Qt Creator debugger orientation

Before diving into scenarios, know where things live:

- **Debug > Start Debugging (F5)** — build (if needed) and launch under the
  debugger.
- **Debug > Windows** — submenu with all the panes used below: **Locals and
  Expressions**, **Stack**, **Breakpoints**, **Threads**, **Modules**,
  **Disassembly**, **Registers**, **Memory**.
- Gutter (left margin of the editor, next to line numbers) — click to
  toggle a breakpoint on that line.
- Toolbar / keyboard shortcuts while stopped at a breakpoint:
  - **F5** — Continue
  - **F10** — Step Over
  - **F11** — Step Into
  - **Shift+F11** — Step Out
  - **Ctrl+F10** — Run to Line (right-click a line and choose "Run to
    Line", or place the cursor and use the menu)
  - **Shift+F5** — Stop Debugging

Keep this section in mind; each scenario below only calls out what's
different or new.

---

## 4. Scenarios

### Scenario 1 — Breakpoints & stepping
**File:** `src/01_breakpoints_stepping.cpp`

1. Open the file and click the gutter next to the line
   `double price = base_price(item);` inside `compute_receipt()`.
2. Start debugging (F5), then type `1` at the menu prompt.
3. When execution stops, use **F10 (Step Over)** to move through
   `apply_discount()` and `add_tax()` without entering them.
4. Restart, but this time use **F11 (Step Into)** on the call to
   `compute_receipt()` from `run_breakpoints_stepping()`, then step into
   `add_tax()` and use **Shift+F11 (Step Out)** to pop back up.
5. Try **Run to Line** on the final `std::println` call.

**What to notice:** Step Over treats a called function as a black box;
Step Into follows execution inside it; Step Out finishes the current
function and returns to its caller. This is the single most important
muscle-memory skill in the whole lab.

---

### Scenario 2 — Locals, watches & the Expressions view
**File:** `src/02_variables_watches.cpp`

1. Set a breakpoint on `std::println("checkpoint A");`.
2. Debug, choose `2`. When stopped, open **Debug > Windows > Locals and
   Expressions**.
3. Expand `player` in Locals — it's a struct; you'll see each member
   (`name`, `health`, `mana`, `position_x`, `position_y`).
4. Right-click `inventory` → **Add to Watch Window** — or just type
   `inventory.size()` in the Expressions box.
5. Add a watch expression: `player.health < 30`. Step forward (F10) and
   watch it stay evaluated live as a boolean.
6. Right-click `player.health` in Locals → **Change Value**, set it to `5`,
   then continue stepping. Watch `is_critical_now` actually flip to `true`
   because you changed state at runtime.

**What to notice:** Locals shows everything in scope automatically;
Expressions/Watches let you track specific values (including expressions,
not just variables) across the entire debug session, even as you step
through unrelated code. Changing a value live is a fast way to test "what
if" branches without recompiling.

---

### Scenario 3 — Call stack & recursion
**File:** `src/03_call_stack_recursion.cpp`

1. Set a breakpoint inside `fibonacci()` on the `return fibonacci(n - 1)
   + fibonacci(n - 2);` line.
2. Debug, choose `3`. Open **Debug > Windows > Stack**.
3. Each time the breakpoint hits, look at the Stack pane — it grows by one
   frame per recursive call. Click different frames to see `n` change at
   each level (Locals updates to match whichever frame is selected).
4. Use **Shift+F11 (Step Out)** repeatedly to unwind and watch return
   values combine.
5. Continue running (or restart) to reach `deeply_nested_call()`, and set a
   breakpoint inside `level_three()`. This time the stack has distinctly
   named frames (`level_three` → `level_two` → `level_one` →
   `deeply_nested_call`) which is closer to what a real bug hunt looks
   like than pure recursion.

**What to notice:** The Stack view is your map of "how did I get here." In
recursive code it can get deep fast — this is intentional, so you get
comfortable navigating many frames instead of just one or two.

---

### Scenario 4 — Pointers, memory view & undefined behavior
**File:** `src/04_pointers_memory.cpp`

1. Set a breakpoint on `int* leaked_ptr = make_dangling();` inside
   `dangling_pointer_demo()`.
2. Debug, choose `4`. Step over (F10) the call. Look at `leaked_ptr` in
   Locals — it holds an address, but the `int` it pointed to
   (`local_value` inside `make_dangling()`) was destroyed the moment that
   function returned. You're looking at a **dangling pointer**.
   (You may notice your compiler already warned about this at build time
   — `C4172` on MSVC, `-Wreturn-local-addr` on GCC/Clang. That's the same
   bug the debugger is showing you, caught two different ways.)
3. Set a second breakpoint at the end of `buffer_overrun_demo()`, on
   `std::println("buffer sum = {}", raw_sum);`. Step to it, then right-click
   `buffer` in Locals → **Open Memory Editor**. You'll see the raw bytes of
   the 10-element array. Toggle between hex and ASCII display.
   - Optional deeper exercise: uncomment the line
     `// buffer[10] = 0xFF;` in the source, rebuild, and re-run with the
     Memory Editor open on `buffer` to watch an out-of-bounds write land
     one slot past the array — a classic off-by-one buffer overrun.
4. Set a breakpoint on `std::println("owner points to {}", *owner);` inside
   `smart_pointer_demo()`. Inspect `owner` (a `std::unique_ptr<int>`) in
   Locals — Qt Creator's pretty-printer shows the pointee value directly
   instead of raw internal pointer bytes.

**What to notice:** The debugger lets you *safely inspect* values that
would be dangerous to actually use in code (like dereferencing
`leaked_ptr`). The Memory Editor is how you see what's really in memory,
byte for byte, which is essential once "the debugger's Locals view" isn't
enough to explain a bug.

---

### Scenario 5 — STL containers & pretty-printers
**File:** `src/05_containers_stl.cpp`

1. Set a breakpoint on the `std::optional<int> maybe_missing = 42;` line.
2. Debug, choose `5`. Expand `scores` (`std::vector<int>`), `name_to_id`
   (`std::map<std::string,int>`), and `fast_lookup`
   (`std::unordered_map<std::string,int>`) in Locals. Compare how each
   renders — vector as an indexed list, map/unordered_map as key/value
   pairs.
3. Step over (F10) to the `maybe_missing.reset();` line, inspect
   `maybe_missing` before and after — the pretty-printer shows "has value"
   vs. empty state directly.
4. Set a breakpoint inside the `for (int s : high_scores)` loop on the
   `std::print("{} ", s);` line. Step through it and watch how
   `high_scores` (a lazily-evaluated `std::ranges` pipeline combining
   `filter` + `transform`) produces values one at a time rather than being
   a materialized container.

**What to notice:** Qt Creator relies on GDB/LLDB/CDB *pretty-printers* to
turn raw container internals into readable views. This is why modern C++
containers are comfortable to debug — you almost never need to manually
decode a vector's internal pointer/size/capacity triplet.

---

### Scenario 6 — Exceptions & the exception breakpoint
**File:** `src/06_exceptions.cpp`

1. Before running, open **Debug > Windows > Breakpoints**. Right-click
   inside the pane → **Add Breakpoint...** → choose a **C++ Exception**
   breakpoint (add it with no filter, so it catches every thrown type).
2. Debug, choose `6`. With the exception breakpoint armed, execution stops
   the instant `throw std::invalid_argument(...)` runs inside `divide()`
   — **before** any unwinding happens, right at the true throw site.
3. Continue (F5) to let it reach the `catch` block; set a normal
   breakpoint there too and inspect the caught `e` in Locals, expanding it
   to see the stored message from `what()`.
4. Remove the exception breakpoint (right-click it → **Delete Breakpoint**)
   and re-run. This time execution jumps straight to the `catch` block
   with no pause at the throw site — you've lost visibility into exactly
   where the exception originated, which is why exception breakpoints
   matter in larger codebases.
5. Look at `nested_rethrow_demo()`: set a breakpoint on the `throw;`
   (bare rethrow) line and watch the Stack view during the second unwind
   as it propagates from the inner catch to the outer one.

**What to notice:** By default, debuggers only stop you at explicit
breakpoints or crashes — not at every throw. An exception breakpoint is
how you catch the *moment* something goes wrong, not just where it's
eventually handled (or not handled at all).

---

### Scenario 7 — Multithreading (Threads view, data races)
**File:** `src/07_multithreading.cpp`

1. Debug, choose `7`, and let `race_condition_demo()` start running. While
   it's executing, click the **Interrupt** (pause) button in the debugger
   toolbar.
2. Open **Debug > Windows > Threads**. You'll see multiple worker threads
   listed, each with its own ID and its own call stack. Click between
   threads — each has independent Locals and a Stack view.
3. Set a breakpoint inside `increment_unsafe()` on the `++unsafe_counter;`
   line and re-run. Notice it's hit repeatedly from **different thread
   IDs** (shown in the Threads pane / breakpoint hit info).
4. Look at the program's own output at the end:
   `unsafe_counter = ... (expected ...) -- mismatch shows the race`. The
   printed value is essentially always lower than expected — proof of a
   real, unsynchronized data race (multiple threads doing a
   non-atomic read-modify-write on the same `int` at once).
5. Compare against `increment_safe()`: set a breakpoint on the
   `std::lock_guard<std::mutex> lock(counter_mutex);` line. Now only one
   thread at a time can be inside the critical section — other threads
   waiting on the mutex show a different (blocked) state if you inspect
   them in the Threads pane while stopped.
6. Optional: open **Debug > Windows > Modules** while stopped to see the
   C++ runtime and threading libraries (e.g. `libstdc++`/`libc++`, or
   `ucrtbase`/threading DLLs on Windows) loaded into the process.

**What to notice:** `unsafe_counter`'s final value is *non-deterministic*
— run the program a few times and the exact wrong number will vary, but
it will essentially never equal the expected total. That's the signature
of a data race. `safe_counter` (atomic) and `mutex_counter` (mutex-guarded)
are always correct.

---

### Scenario 8 — Templates & generic code (C++23)
**File:** `src/08_templates_generic.cpp`

1. Set a breakpoint inside `sum_all()` on the `total += v;` line.
2. Debug, choose `8`. Step in from the `sum_all(ints)` call site first —
   check the Stack view: it shows the concrete instantiation, e.g.
   `sum_all<int>`, not a generic "sum_all". Continue, and step in again
   from the `sum_all(doubles)` call — now the stack shows `sum_all<double>`.
   Same source line, two different compiled functions.
3. Set a breakpoint inside `Box<T>::describe()` (uses C++23 "deducing
   this"). Inspect the `self` parameter in Locals — it stands in for
   `this`, deduced per call site.
4. Set a breakpoint inside `classify()` on the runtime branch (the `return
   n < 0 ? -1 : (n == 0 ? 0 : 1);` line inside the `else`). Notice you can
   never break inside the `if consteval` branch above it with a live
   debugger — if that branch were selected, it would have run entirely at
   compile time and produced no runtime code to stop in.

**What to notice:** Templates are compiled per instantiation — the
debugger always shows you the concrete, generated function for the types
you actually used, which is why the same source line can appear under
different "instantiated" names in the Stack view.

---

### Scenario 9 — Conditional & logging breakpoints in a hot loop
**File:** `src/09_conditional_breakpoints.cpp`

This scenario loops 1000 times; exactly once (`i == 733`) a helper
function returns a dangerous value. Finding that one iteration by
stepping 1000 times would be miserable — which is the point.

1. Set a breakpoint on `int result = value / divisor;`.
2. Right-click the breakpoint marker (in the gutter or in the
   **Breakpoints** pane) → **Edit Breakpoint...** and set **Condition** to
   `divisor == 0`. Debug, choose `9` — the debugger now skips straight to
   the one iteration where the condition is true, instead of stopping
   1000 times.
3. Alternative: set the condition to `i == 733` instead, to jump by loop
   index rather than by value.
4. Alternative approach — **ignore count**: remove the condition, and
   instead right-click the breakpoint → set **Skip N times** (Ignore
   Count) to `732`. This achieves the same "jump to iteration 733" result
   a different way.
5. Try a **logging breakpoint** (tracepoint): Edit the breakpoint, enable
   its message/log action with text like `i={i} divisor={divisor}`, and
   untick "stop on hit" if present. Re-run — the Debugger Console /
   Application Output prints a trace line per iteration without ever
   halting execution, letting you spot patterns without single-stepping
   at all.

**What to notice:** Conditional breakpoints, ignore counts, and logging
breakpoints all solve the same problem — a bug buried deep in a loop or
called from thousands of call sites — without you having to manually step
past every uninteresting hit.

---

### Scenario 10 — Debug vs. Release: optimized-build debugging quirks
**File:** `src/10_optimized_release_debugging.cpp`

Every other scenario runs from a normal **Debug** build (unoptimized, full
debug info) — the default this project configures. This scenario is best
understood by comparing that Debug build against a second, **optimized**
build of the exact same source.

1. In Qt Creator, open **Projects** mode (left sidebar) → your kit → **Build**.
2. Click **Add** under build configurations and choose a new configuration
   based on **RelWithDebInfo** (or duplicate the existing one and change
   its `CMAKE_BUILD_TYPE` variable to `RelWithDebInfo` under Initial/Current
   Configuration). This build type is compiler-agnostic: CMake maps it to
   "optimizations on, but debug info still generated" automatically for
   MSVC, GCC, and Clang alike — no manual flags needed.
3. Reconfigure and build this new configuration.
4. Set a breakpoint inside `inefficient_sum()` on the `subtotal += j;`
   line.
5. Debug the **Debug** build first, choose `10`, and step through
   normally — every loop iteration is visible exactly as written, and
   `subtotal` behaves predictably in Locals.
6. Switch the active build configuration (**Projects** mode → select the
   RelWithDebInfo configuration, or use the build configuration selector
   at the bottom-left of the Qt Creator window), rebuild, and debug again
   with the **same breakpoint**.
7. Compare what you see: in the optimized build, the breakpoint may be hit
   far fewer times than you'd expect (loop unrolling/inlining), and
   `subtotal` may show as **`<optimized out>`** in Locals, because the
   compiler kept it purely in a CPU register with no stable memory
   address to inspect.
8. Optional: open **Debug > Windows > Disassembly** (or right-click in the
   editor → **Show Disassembly**) for `inefficient_sum` in both builds and
   compare instruction counts.

**What to notice:** This is one of the most common "why doesn't the
debugger match my code?!" moments in real-world C++ development.
Optimized builds are harder to debug because the compiler has
legally rearranged, inlined, or eliminated things your source code
implies exist. When a bug only reproduces in Release builds, expect
exactly this kind of friction.

---

## 5. Suggested order for a first pass

If you're working through this linearly (e.g. for a video/course), this
order builds skills progressively:

1. Scenario 1 (stepping fundamentals)
2. Scenario 2 (inspecting/changing state)
3. Scenario 3 (reading the call stack)
4. Scenario 9 (conditional breakpoints — once stepping alone gets tedious)
5. Scenario 4 (memory & undefined behavior)
6. Scenario 5 (STL pretty-printers)
7. Scenario 6 (exceptions)
8. Scenario 8 (templates)
9. Scenario 7 (multithreading — most advanced view usage)
10. Scenario 10 (Debug vs. Release — ties everything together)

## 6. Project layout

```
01-qt-creator-debugging/
├── CMakeLists.txt                          # C++23, single executable target
├── README.md                               # this file
└── src/
    ├── main.cpp                            # interactive scenario menu
    ├── scenarios.h                         # declares one entry point per scenario
    ├── 01_breakpoints_stepping.cpp
    ├── 02_variables_watches.cpp
    ├── 03_call_stack_recursion.cpp
    ├── 04_pointers_memory.cpp
    ├── 05_containers_stl.cpp
    ├── 06_exceptions.cpp
    ├── 07_multithreading.cpp
    ├── 08_templates_generic.cpp
    ├── 09_conditional_breakpoints.cpp
    └── 10_optimized_release_debugging.cpp
```

Each `.cpp` file's top comment block repeats the key steps for that
scenario, so once the project is open in Qt Creator you have the
instructions right next to the code they refer to.
