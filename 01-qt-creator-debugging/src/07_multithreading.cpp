#include <atomic>
#include <mutex>
#include <print>
#include <thread>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 07 - Multithreading (Threads view, data races)
//
// What to try in Qt Creator:
//   * Make sure the Threads view is visible: go to View > Views and
//     check "Threads" if it isn't already.
//   * Start debugging (F5) and let race_condition_demo() start running.
//     While it's executing, go to Debug > Interrupt (there is no default
//     keyboard shortcut for this) to pause it mid-flight. Look at the
//     Threads view: you'll see multiple worker threads listed, each with
//     its own thread id. Click between them -- the Stack and Locals
//     views update to show that specific thread's own call stack and
//     variables.
//   * Continue (F5), then set a breakpoint inside increment_unsafe() on
//     the `++unsafe_counter;` line and run again. Notice the breakpoint
//     is hit repeatedly from *different* thread ids (shown in the
//     Threads view / in the breakpoint hit info) -- several threads are
//     all racing to run this same line at once.
//   * Look at the program's own printed output at the end:
//     "unsafe_counter = ... (expected ...) -- mismatch shows the race".
//     The two numbers essentially never match, because multiple threads
//     read-modify-wrote `unsafe_counter` at the same time without any
//     synchronization, and some of those updates were silently lost.
//   * Compare against increment_safe(), which uses a std::mutex. Set a
//     breakpoint on the `std::scoped_lock lock(counter_mutex);` line
//     and run again: only one thread at a time is ever stopped there,
//     because the others are blocked waiting for the mutex -- you can
//     confirm this by checking their state in the Threads view.
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
