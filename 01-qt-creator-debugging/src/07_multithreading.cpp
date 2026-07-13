#include <atomic>
#include <mutex>
#include <print>
#include <thread>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 07 - Multithreading (Threads view, data races, deadlocks)
//
// What to try in Qt Creator:
//   * Run race_condition_demo() under the debugger and open View >
//     Debugger Windows > Threads. While it's running, hit Interrupt (the
//     pause button); you'll see multiple worker threads listed, each with
//     its own stack. Click between threads to see each one's independent
//     call stack and locals.
//   * Set a breakpoint inside increment_unsafe() and re-run: notice it's
//     hit repeatedly from *different* thread IDs, and the "unsafe_counter"
//     value can be inconsistent with how many times you'd expect given the
//     number of hits, because of the race.
//   * Compare against increment_safe(), which takes a std::mutex; set a
//     breakpoint on the `std::lock_guard` line and watch only one thread
//     be inside the critical section at a time (others block, visible as
//     a different thread state in the Threads view).
//   * Try View > Debugger Windows > "Modules" while stopped to see
//     libstdc++/libc++ and the pthread/Win32 thread libraries loaded.
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
        std::lock_guard<std::mutex> lock(counter_mutex); // <-- breakpoint here
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
    std::println("unsafe_counter = {} (expected {}) -- mismatch shows the race", unsafe_counter, expected);
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
    std::println("-- unsynchronized race demo (result is non-deterministic on purpose) --");
    race_condition_demo();

    std::println("-- synchronized demo (atomic + mutex, always correct) --");
    synchronized_demo();
}
