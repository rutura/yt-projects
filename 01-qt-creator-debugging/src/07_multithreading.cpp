#include <atomic>
#include <mutex>
#include <print>
#include <thread>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 07 - Multithreading (Threads view, data races)
//
// -----------------------------------------------------------------------
// Concepts first: what is a thread?
// -----------------------------------------------------------------------
// A normal, single-threaded program is one instruction pointer moving
// through code, start to finish:
//
//     main() ----------------------------------------------------> end
//
// A multithreaded program starts several *independent* instruction
// pointers that all run at the same time and all share the same memory
// (globals, heap objects, etc). Here, race_condition_demo() below spins
// up kThreadCount (4) worker threads that all call increment_unsafe():
//
//     main thread
//        |
//        |-- spawn --> Thread 1: increment_unsafe() ---\
//        |-- spawn --> Thread 2: increment_unsafe() ----\
//        |-- spawn --> Thread 3: increment_unsafe() ----- all running
//        |-- spawn --> Thread 4: increment_unsafe() ----/ at once
//        |                                              /
//        |<----------------- join() waits for all -----/
//        v
//     main thread continues, prints result
//
// Each thread runs the exact same function, on its own independent path
// through it, but they all read and write the *same* `unsafe_counter`
// variable, because it's declared outside any function (a shared global).
//
// -----------------------------------------------------------------------
// Why sharing memory is dangerous: the data race
// -----------------------------------------------------------------------
// `++unsafe_counter;` looks like a single step in the source code, but
// the CPU actually performs three separate steps to do it:
//
//     1. READ  unsafe_counter from memory into a register
//     2. ADD   1 to that register
//     3. WRITE the register's new value back to memory
//
// If two threads interleave those three steps badly, an update gets
// silently lost. Picture both threads starting from unsafe_counter == 5:
//
//     time -->
//     Thread 1:   READ (5)   ADD -> 6            WRITE 6
//     Thread 2:              READ (5)   ADD -> 6            WRITE 6
//     -----------------------------------------------------------------
//     unsafe_counter:  5        5         5         6          6
//
// Both threads read `5` before either wrote back, so *one entire
// increment vanished* -- the counter should be 7, but it's 6. This is a
// "data race": two or more threads accessing the same memory at the same
// time, with at least one of them writing, and no synchronization to
// keep their steps from interleaving. It's undefined behavior in C++,
// and it's exactly what increment_unsafe() below triggers, 100,000 times
// per thread, 4 threads at once -- so plenty of increments get lost.
//
// -----------------------------------------------------------------------
// Two ways to fix it (both demonstrated below)
// -----------------------------------------------------------------------
// Fix 1: std::atomic (see increment_atomic()). An atomic variable makes
// the whole read-modify-write sequence indivisible at the hardware level
// -- no other thread can ever observe it "half done". Cheap, no waiting.
//
// Fix 2: std::mutex (see increment_safe()). A mutex is a lock: only one
// thread may hold it at a time. `std::scoped_lock lock(counter_mutex);`
// grabs the lock and automatically releases it when `lock` goes out of
// scope at the end of the loop body. Everyone else who wants the lock
// simply waits (blocks) until it's free:
//
//     Thread 1:  [lock]-->[++mutex_counter]-->[unlock]
//     Thread 2:      ...waiting...                    [lock]-->[++mutex_counter]-->[unlock]
//
// This is slower than atomic (threads sit idle instead of working), but
// it generalizes to protecting more than one variable / a whole block of
// code at once, which a single atomic cannot do.
//
// -----------------------------------------------------------------------
// What to try in Qt Creator
// -----------------------------------------------------------------------
//   * Make sure the Threads view is visible: go to View > Views and
//     check "Threads" if it isn't already.
//   * Set a breakpoint on the `++unsafe_counter;` line inside
//     increment_unsafe() below.
//   * Start debugging (F5) and choose this scenario. Execution stops the
//     first time *any* thread reaches that line.
//   * Open the Threads view and look at its `Function` / `File` / `Line`
//     columns. You will see MORE rows than just your 4 workers -- CDB/
//     LLDB also lists internal OS/runtime threads (functions like
//     NtWaitForSingleObject or NtWaitForWorkViaWorkerFactory, with no
//     File/Line of their own). Ignore those; they're not your code.
//     The rows that matter are the ones whose Function names
//     increment_unsafe and whose Line reads 148 (or wherever your
//     breakpoint is) -- those are your worker threads. Right after the
//     very first stop you'll often see just 1, because the other workers
//     haven't reached the breakpoint yet.
//   * Press F5 (Continue) a handful of times (5-10 presses is plenty).
//     Each press resumes every thread until *any one* of them hits the
//     breakpoint again, so you're sampling the race, not single-stepping
//     it. Re-check the Threads view after each press -- watch the count
//     of increment_unsafe rows grow as more workers arrive, and their
//     thread ids change as different threads take turns being the one
//     that's currently stopped. Once you've seen up to kThreadCount (4)
//     of them and watched several different thread ids get hit, you've
//     seen everything this breakpoint has to show you. (You may also
//     eventually see FEWER rows than before, if a worker finished its
//     entire 100,000-iteration loop and exited between presses -- that's
//     expected too.)
//   * Click one of the increment_unsafe rows, then another. Watch the
//     toolbar's thread selector (top of the Debug pane) and the
//     highlighted row both switch to match -- that confirms you're now
//     looking at that specific thread. The Stack view will look
//     IDENTICAL between them: same frames, same line, same depth. That's
//     expected, not a sign nothing happened -- increment_unsafe() isn't
//     recursive, so every worker's call stack has the exact same shape.
//     What actually differs per-thread is the *value*, not the shape:
//     check the Locals pane for `i` (the loop counter) while each thread
//     is selected -- each thread has its own independent `i`, frozen at
//     wherever that particular thread's loop happened to be when it hit
//     the breakpoint.
//   * Now remove the breakpoint (or disable it) -- this loop runs
//     400,000 total increments, and you obviously can't press F5 that
//     many times by hand. With the breakpoint gone, press F5 once more
//     to let the program run to completion at full speed, and read its
//     own printed output:
//     "unsafe_counter = ... (expected ...) -- mismatch shows the race".
//     The two numbers essentially never match, because multiple threads
//     read-modify-wrote `unsafe_counter` at the same time without any
//     synchronization, and some of those updates were silently lost.
//   * Compare against increment_safe(), which uses a std::mutex. Set a
//     breakpoint on the `std::scoped_lock lock(counter_mutex);` line
//     and debug again: only one thread at a time is ever stopped there,
//     because the others are blocked waiting for the mutex -- check the
//     Threads view and you'll see the rest sitting in a waiting/blocked
//     state instead of also being stopped at your breakpoint.
// ---------------------------------------------------------------------------

namespace {

constexpr int kThreadCount = 4;
constexpr int kIncrementsPerThread = 100'000;

int unsafe_counter = 0;         // no synchronization: intentional data race
std::atomic<int> safe_counter{0};
int mutex_counter = 0;
std::mutex counter_mutex;

void increment_unsafe() {
    for (int i = 0; i < kIncrementsPerThread; ++i) {
        ++unsafe_counter; // <-- breakpoint here: hit from many threads, racily
    }
}

void increment_atomic() {
    for (int i = 0; i < kIncrementsPerThread; ++i) {
        ++safe_counter;
    }
}

void increment_safe() {
    for (int i = 0; i < kIncrementsPerThread; ++i) {
        std::scoped_lock lock(counter_mutex); // <-- breakpoint here
        ++mutex_counter;
    }
}

void race_condition_demo() {
    std::vector<std::thread> workers;
    workers.reserve(kThreadCount);
    for (int i = 0; i < kThreadCount; ++i) {
        workers.emplace_back(increment_unsafe);
    }

    // Wait for all threads to finish before printing the result
    for (auto& t : workers) {
        t.join();
    }

    const int expected = kThreadCount * kIncrementsPerThread;
    std::println("unsafe_counter = {} (expected {}) -- mismatch shows the race",
                  unsafe_counter, expected);
}

void synchronized_demo() {
    std::vector<std::thread> workers;
    workers.reserve(kThreadCount);
    for (int i = 0; i < kThreadCount; ++i) {
        workers.emplace_back(increment_atomic);
    }
    for (auto& t : workers) {
        t.join();
    }
    std::println("safe_counter (atomic) = {}", safe_counter.load());

    workers.clear();
    for (int i = 0; i < kThreadCount; ++i) {
        workers.emplace_back(increment_safe);
    }
    for (auto& t : workers) {
        t.join();
    }
    std::println("mutex_counter = {}", mutex_counter);
}

} // namespace

void scenarios::run_multithreading() {
    std::println("-- unsynchronized race demo --");
    std::println("(result is non-deterministic, on purpose)");
    race_condition_demo();

    std::println("-- synchronized demo (atomic + mutex, always correct) --");
    synchronized_demo();
}
