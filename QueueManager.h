#pragma once
#include <mutex>

struct PartyRequirement {
    int tanks = 1;
    int healers = 1;
    int dps = 3;
};

class QueueManager {
public:
    QueueManager(int t, int h, int d);

    // it will return true if the queue can form a party
    bool canFormParty(const PartyRequirement& req = PartyRequirement{}) const;

    // consum players to form a party if possible. 
    // will true if succeeded.
    bool formParty(const PartyRequirement& req = PartyRequirement{});

    // accessors (thread-safe)
    int tanks() const;
    int healers() const;
    int dps() const;

    // bonus feature 
    void addPlayers(int t_add, int h_add, int d_add);

private:
    mutable std::mutex mtx;
    int t, h, d;
};
