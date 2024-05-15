#ifndef DPLL_CNF_H
#define DPLL_CNF_H

#include <vector>
#include <string>
#include <unordered_set>

namespace sat_solver {

    class CNF {
    public:
        friend bool dpll(CNF *, std::vector<int> &);
        /// Parses DIMACS CNF formatted file, splitting clause into multiple lines is not supported
        static CNF *parse(const std::string &);
        bool unit_propagate();
        bool pure_literals_eliminate();
        [[nodiscard]] std::unordered_set<int> find_pure_literals() const;
        [[nodiscard]] std::unordered_set<int> find_unit_clauses() const;
        void model_dimacs_output();
        [[nodiscard]] int choose_literal() const;
    private:
        CNF(int vars_num, int clauses_num) : vars_num(vars_num), clauses_num(clauses_num) {}
        void eliminate_clauses_with_certain_literals(const std::unordered_set<int> &);
        void eliminate_neg_literals_from_clauses(const std::unordered_set<int> &);
        int vars_num; // is needed to check if num is small enough, then not being touched at all
        int clauses_num;
        bool contains_empty_clause = false;
        std::vector<int> model;
        std::vector<std::unordered_set<int>> clauses;
    };

} // sat_solver

#endif //DPLL_CNF_H
