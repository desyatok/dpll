#ifndef DPLL_CNF_H
#define DPLL_CNF_H

#include <vector>
#include <string>
#include <unordered_set>

namespace sat_solver {

    class CNF {
        int vars_num;
        int clauses_num;
        bool empty_cnf = false;
        std::vector<std::unordered_set<int>> clauses;
        CNF(int vars_num, int clauses_num) : vars_num(vars_num), clauses_num(clauses_num), clauses() {}

    public:
        CNF() = delete;
        /// Parses DIMACS CNF formatted file, splitting clause into multiple lines is not supported
        static CNF *parse(const std::string &input);
        bool unit_propagate();
        bool pure_literals_eliminate();
        std::unordered_set<int> find_pure_literals() const;

    };

} // sat_solver

#endif //DPLL_CNF_H
