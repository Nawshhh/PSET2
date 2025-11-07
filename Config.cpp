#include "Config.h"
#include "Utils.h"
#include <fstream>
#include <map>
#include <stdexcept>
#include <unordered_set>
#include <sstream>

static inline void failLine(int lineno, const std::string& why) {
    std::ostringstream oss;
    if (lineno > 0) oss << "config parse error at line " << lineno << ": ";
    oss << why;
    throw std::runtime_error(oss.str());
}

Config Config::FromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Failed to open config file: " + path);

    const std::unordered_set<std::string> allowed{"n","t","h","d","t1","t2"};

    std::map<std::string,int> kv;
    std::string line;
    int lineno = 0;

    while (std::getline(in, line)) {
        ++lineno;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        auto pos = line.find('=');
        if (pos == std::string::npos) failLine(lineno, "missing '='");

        std::string key = trim(line.substr(0, pos));
        std::string val = trim(line.substr(pos + 1));

        if (key.empty())                     failLine(lineno, "empty key");
        if (!allowed.count(key))             failLine(lineno, "unknown key '" + key + "' (allowed: n,t,h,d,t1,t2)");
        if (val.empty())                     failLine(lineno, "empty value for key '" + key + "'");
        if (!val.empty() && val[0] == '-')   failLine(lineno, "negative values not allowed for key '" + key + "'");

        int iv;
        if (!parseInt(val, iv))              failLine(lineno, "value must be an integer (no floats) for key '" + key + "'");
        if (iv < 0)                          failLine(lineno, "negative values not allowed for key '" + key + "'");

        kv[key] = iv;
    }

    // --- Require ALL keys present ---
    const char* required[] = {"n","t","h","d","t1","t2"};
    std::vector<std::string> missing;
    for (auto* k : required) if (!kv.count(k)) missing.emplace_back(k);
    if (!missing.empty()) {
        std::ostringstream oss;
        oss << "missing required key(s): ";
        for (size_t i = 0; i < missing.size(); ++i) {
            if (i) oss << ", ";
            oss << missing[i];
        }
        throw std::runtime_error(oss.str());
    }

    // --- Assign + semantic validation ---
    Config cfg;
    cfg.n_instances = kv["n"];
    cfg.tanks       = kv["t"];
    cfg.healers     = kv["h"];
    cfg.dps         = kv["d"];
    cfg.time1       = kv["t1"];
    cfg.time2       = kv["t2"];

    if (cfg.n_instances < 1)  failLine(0, "n must be >= 1");
    if (cfg.tanks   < 0 || cfg.healers < 0 || cfg.dps < 0)
                               failLine(0, "t,h,d must be >= 0");
    if (cfg.time1   < 1)       failLine(0, "t1 must be >= 1");
    if (cfg.time2   < cfg.time1)
                               failLine(0, "t2 must be >= t1");

    return cfg;
}
