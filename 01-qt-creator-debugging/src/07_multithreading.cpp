#include <atomic>
#include <mutex>
#include <print>
#include <thread>
#include <vector>

#include "scenarios.h"

// ---------------------------------------------------------------------------
// Scenario 07 - Multithreading (Threads view, data races)
//
// race_condition_demo() below starts kThreadCount (4) threads that all
// call increment_unsafe() at once. They all read, add 1, and write back
// the same shared `unsafe_counter` with no synchronization -- so when
// two threads interleave those three steps badly, an update is lost:
//
//     Thread 1:   READ (5)   ADD -> 6            WRITE 6
//     Thread 2:              READ (5)   ADD -> 6            WRITE 6
//     unsafe_counter ends up 6, not 7 -- one increment vanished.
//
// That's a "data race", and it's why the count printed at the end never
// matches the expected total. increment_atomic() (std::atomic) and
// increment_safe() (std::mutex) fix it two different ways -- see them
// below.
//
// Full explanation (what a thread is, why the race happens, atomic vs.
// mutex, and step-by-step debugger instructions -- including the exact
// Threads view quirks you'll hit on real hardware) lives in the project
// README.md, under "Scenario 7 -- Multithreading". Read that before
// debugging this file.
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
