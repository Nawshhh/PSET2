#pragma once
#include <string>

class Config {
public:
    Config() = default;

    static Config FromFile(const std::string& path);

    int n()  const { return n_instances; }
    int t()  const { return tanks; }
    int h()  const { return healers; }
    int d()  const { return dps; }
    int t1() const { return time1; }
    int t2() const { return time2; }

private:
    int n_instances = 1;
    int tanks = 0;
    int healers = 0;
    int dps = 0;
    int time1 = 1;
    int time2 = 5;
};
