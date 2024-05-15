#include <iostream>
#include <memory>
#include "DPLL.h"

using namespace sat_solver;
int main()
{
    CNF *cnf = CNF::parse("my.cnf");
    dpll(cnf);
    delete cnf;
    return 0;
}
