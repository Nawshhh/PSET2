#pragma once
#include <string>

class Config {
public:
    Config() = default;

    static Config FromFile(const std::string& path);

    int n()  const { return n_instances_; }
    int t()  const { return tanks_; }
    int h()  const { return healers_; }
    int d()  const { return dps_; }
    int t1() const { return t1_; }
    int t2() const { return t2_; }

private:
    int n_instances_ = 1;
    int tanks_ = 0;
    int healers_ = 0;
    int dps_ = 0;
    int t1_ = 1;
    int t2_ = 5;
};
