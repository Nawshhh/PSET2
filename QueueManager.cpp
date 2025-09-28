#include "QueueManager.h"

QueueManager::QueueManager(int t, int h, int d) : t(t), h(h), d(d) {}

bool QueueManager::canFormParty(const PartyRequirement& req) const {
    std::lock_guard<std::mutex> lock(mtx);
    return (t >= req.tanks) && (h >= req.healers) && (d >= req.dps);
}

bool QueueManager::formParty(const PartyRequirement& req) {
    std::lock_guard<std::mutex> lock(mtx);
    if ((t >= req.tanks) && (h >= req.healers) && (d >= req.dps)) {
        t -= req.tanks;
        h -= req.healers;
        d -= req.dps;
        return true;
    }
    return false;
}

int QueueManager::tanks() const { std::lock_guard<std::mutex> lock(mtx); return t; }
int QueueManager::healers() const { std::lock_guard<std::mutex> lock(mtx); return h; }
int QueueManager::dps() const { std::lock_guard<std::mutex> lock(mtx); return d; }
