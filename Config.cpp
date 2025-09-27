#include "Config.h"
#include "Utils.h"
#include <fstream>
#include <map>
#include <stdexcept>

Config Config::FromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Failed to open config file: " + path);

    std::map<std::string,int> kv;
    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;
        std::string key = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));
        int iv;
        if (parseInt(val, iv)) kv[key] = iv;
    }

    Config cfg;
    if (kv.count("n"))  cfg.n_instances_ = kv["n"];
    if (kv.count("t"))  cfg.tanks_       = kv["t"];
    if (kv.count("h"))  cfg.healers_     = kv["h"];
    if (kv.count("d"))  cfg.dps_         = kv["d"];
    if (kv.count("t1")) cfg.t1_          = kv["t1"];
    if (kv.count("t2")) cfg.t2_          = kv["t2"];
    if (cfg.t1_ < 1) cfg.t1_ = 1;
    if (cfg.t2_ < cfg.t1_) cfg.t2_ = cfg.t1_;
    return cfg;
}
