#pragma once
#include <atomic>
#include <mutex>
#include <thread>
#include <string>
#include <chrono>

class Instance {
public:
    explicit Instance(int id);
    ~Instance();

    // non copyable
    Instance(const Instance&) = delete;
    Instance& operator=(const Instance&) = delete;

    bool isIdle() const;                  // true if no party is running
    std::string statusString() const;     // "active" or "empty"

    // try to assign a run with the given duration (seconds).
    // will return true if the instance accepted the run, false if it was busy.
    bool assignRun(int durationSeconds);

    // stats
    int servedCount() const;
    long long totalTimeSeconds() const;

private:
    void runWorker(int durationSeconds);

    int id;
    mutable std::mutex mtx;
    std::atomic<bool> active;
    std::thread worker;
    std::atomic<int> served_count;
    std::atomic<long long> total_time_seconds;
};
