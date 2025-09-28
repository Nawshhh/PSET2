#pragma once
#include <vector>
#include <string>
#include <random>
#include <memory>
#include "Config.h"

class Instance;
class QueueManager;

class LFGSimulator {
public:
    explicit LFGSimulator(const Config& cfg);
    ~LFGSimulator();             // out-of-line, so unique_ptr sees complete types
    void run();                  // prints status + summary

private:
    void printStatus() const;
    bool allIdle() const;

    Config cfg_;
    std::vector<std::unique_ptr<Instance>> instances;
    std::unique_ptr<QueueManager> queue;
    mutable std::mt19937 rng;
};
