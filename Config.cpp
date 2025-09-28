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
    if (kv.count("n"))  cfg.n_instances = kv["n"];
    if (kv.count("t"))  cfg.tanks       = kv["t"];
    if (kv.count("h"))  cfg.healers     = kv["h"];
    if (kv.count("d"))  cfg.dps         = kv["d"];
    if (kv.count("t1")) cfg.time1          = kv["t1"];
    if (kv.count("t2")) cfg.time2          = kv["t2"];
    if (cfg.time1 < 1) cfg.time1 = 1;
    if (cfg.time2 < cfg.time1) cfg.time2 = cfg.time1;
    return cfg;
}
