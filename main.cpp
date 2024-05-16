#include <iostream>
#include <memory>
#include "DPLL.h"

constexpr auto FAILURE = -1;
using namespace sat_solver;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Exactly one dimacs-formatted file should be provided\n";
        return FAILURE;
    }
    CNF *cnf = CNF::parse(argv[1]);
    dpll(cnf);

    delete cnf;
    return 0;
}
