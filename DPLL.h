#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H

#include "CNF.h"

namespace sat_solver {

    bool dpll_helper(CNF *);
    void dpll(CNF *);

} // sat_solver

#endif //DPLL_DPLL_H
