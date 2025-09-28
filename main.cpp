#include "LFGSimulator.h"
#include "Config.h"
#include <iostream>

int main(int argc, char** argv) {
    std::string cfgPath = "config.txt";
    if (argc >= 2) cfgPath = argv[1];

    try {
        Config cfg = Config::FromFile(cfgPath);
        LFGSimulator sim(cfg);
        sim.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
