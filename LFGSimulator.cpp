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
    std::uniform_int_distribution<int> roleDist(0, 2);          // 0=T, 1=H, 2=D
    std::uniform_int_distribution<int> sleepMs(300, 900);        // 0.3–0.9 s between arrivals

    cout << "=== LFG Dungeon Queue Simulator ===\n";
    cout << "Instances: " << cfg_.n()
         << " | Queue: Tanks = " << cfg_.t()
         << " Healers =" << cfg_.h()
         << " DPS =" << cfg_.d()
         << " | Duration: [" << cfg_.t1() << "," << cfg_.t2() << "] sec\n\n";

    std::atomic<bool> stop{false};
    std::atomic<bool> arrivals_done{false};

    // BONUS: producer thread — randomly injects players into the queue
    std::thread producer([&]{
        const int initial_total = queue->tanks() + queue->healers() + queue->dps();
        const int arrivals_target = std::max(1, initial_total / 2);  

        for (int i = 0; i < arrivals_target && !stop.load(); ++i) {
            int r = roleDist(rng);              
            int t_add = (r == 0) ? 1 : 0;
            int h_add = (r == 1) ? 1 : 0;
            int d_add = (r == 2) ? 1 : 0;

            queue->addPlayers(t_add, h_add, d_add);

            std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs(rng)));
        }
        arrivals_done = true;
    });

    // launch parties onto idle instances while possible
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
            if (!queue->canFormParty() && allIdle() && arrivals_done) {
                stop = true;
            }
        }
    });


    while (!stop.load()) {
        printStatus();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    dispatcher.join();
    stop = true;                                 // make sure producer exits if still looping
    if (producer.joinable()) producer.join();

    // drain until all instance threads are idle
    while (!allIdle()) {
        printStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    printStatus();

    // stats
    cout << "\n=== Summary ===\n";
    long long totalServed = 0;
    long long totalTime = 0;
    for (size_t i = 0; i < instances.size(); ++i) {
        auto served = instances[i]->servedCount();
        auto time = instances[i]->totalTimeSeconds();
        totalServed += served;
        totalTime += time;
        cout << "Instance " << setw(2) << i
             << " | Parties: " << setw(3) << served
             << " | Time: " << setw(3) << time << "s\n";
    }
    cout << "Total parties served: " << totalServed << "\n";
    cout << "Aggregate time served: " << totalTime << "s\n";
    cout << "Remaining in queue: Tanks =" << queue->tanks()
         << " Healers =" << queue->healers()
         << " DPS =" << queue->dps() << "\n";
}
