#include "Config.h"
#include <iostream>

int main(int argc, char** argv) {
    std::string cfgPath = "config.txt";
    if (argc >= 2) cfgPath = argv[1];

    try {
        Config cfg = Config::FromFile(cfgPath);
        std::cout << "Instances: " << cfg.n() << "\n";
        std::cout << "Tanks: "     << cfg.t() << "\n";
        std::cout << "Healers: "   << cfg.h() << "\n";
        std::cout << "DPS: "       << cfg.d() << "\n";
        std::cout << "Level range: " << cfg.t1() << " - " << cfg.t2() << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
