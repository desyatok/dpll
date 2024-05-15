#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H

#include "CNF.h"

namespace sat_solver {

    bool dpll(CNF *, std::vector<int> &);

} // sat_solver

#endif //DPLL_DPLL_H
