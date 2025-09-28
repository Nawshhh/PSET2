#include "Instance.h"
#include <iostream>

Instance::Instance(int id)
    : id(id), active(false), served_count(0), total_time_seconds(0) {}

Instance::~Instance() {
    if (worker.joinable()) worker.join();
}

bool Instance::isIdle() const {
    return !active.load();
}

std::string Instance::statusString() const {
    return isIdle() ? "empty" : "active";
}

bool Instance::assignRun(int durationSeconds) {
    std::lock_guard<std::mutex> lock(mtx);
    if (active) return false;
    active = true;

    // worker thread
    worker = std::thread(&Instance::runWorker, this, durationSeconds);
    worker.detach(); // instance owns its state
    return true;
}

void Instance::runWorker(int durationSeconds) {
    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::seconds(durationSeconds));
    auto end = std::chrono::steady_clock::now();
    long long elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    total_time_seconds.fetch_add(elapsed);
    served_count.fetch_add(1);

    // mark it as idle
    active = false;
}

int Instance::servedCount() const {
    return served_count.load();
}

long long Instance::totalTimeSeconds() const {
    return total_time_seconds.load();
}
