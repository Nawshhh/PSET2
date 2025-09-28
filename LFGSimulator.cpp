#include "LFGSimulator.h"
#include "Instance.h"
#include "QueueManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

LFGSimulator::LFGSimulator(const Config& cfg)
    : cfg_(cfg), rng(std::random_device{}()) {
    instances.reserve(cfg_.n());
    for (int i = 0; i < cfg_.n(); ++i) {
        instances.emplace_back(new Instance(i));
    }
    queue.reset(new QueueManager(cfg_.t(), cfg_.h(), cfg_.d()));
}

LFGSimulator::~LFGSimulator() = default;

bool LFGSimulator::allIdle() const {
    for (auto& inst : instances) {
        if (!inst->isIdle()) return false;
    }
    return true;
}

void LFGSimulator::printStatus() const {
    cout << "[Status] ";
    for (size_t i = 0; i < instances.size(); ++i) {
        cout << "Instance " << i << ": " << instances[i]->statusString();
        if (i + 1 != instances.size()) cout << " | ";
    }
    cout << '\n';
}

void LFGSimulator::run() {
    std::uniform_int_distribution<int> dist(cfg_.t1(), cfg_.t2());
    cout << "=== LFG Dungeon Queue Simulator ===\n";
    cout << "Instances: " << cfg_.n() << " | Queue: T=" << cfg_.t() << " H=" << cfg_.h() << " D=" << cfg_.d() << " | Duration: [" << cfg_.t1() << "," << cfg_.t2() << "] sec\n\n";

    std::atomic<bool> stop{false};
    std::thread dispatcher([&]{
        while (!stop.load()) {
            bool launched = false;
            for (auto& inst : instances) {
                if (inst->isIdle() && queue->formParty()) {
                    int dur = dist(rng);
                    inst->assignRun(dur);
                    launched = true;
                    break;
                }
            }
            if (!launched) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            if (!queue->canFormParty() && allIdle()) {
                stop = true;
            }
        }
    });

    while (!stop.load()) {
        printStatus();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    dispatcher.join();

    while (!allIdle()) {
        printStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    printStatus();

    cout << "\n=== Summary ===\n";
    long long totalServed = 0;
    long long totalTime = 0;
    for (size_t i = 0; i < instances.size(); ++i) {
        auto served = instances[i]->servedCount();
        auto time = instances[i]->totalTimeSeconds();
        totalServed += served;
        totalTime += time;
        cout << "Instance " << setw(2) << i << " | Parties: " << setw(3) << served << " | Time: " << setw(3) << time << "s\n";
    }
    cout << "Total parties served: " << totalServed << "\n";
    cout << "Aggregate time served: " << totalTime << "s\n";
    cout << "Remaining in queue: T=" << queue->tanks() << " H=" << queue->healers() << " D=" << queue->dps() << "\n";
}
