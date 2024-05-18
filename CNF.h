#ifndef DPLL_CNF_H
#define DPLL_CNF_H

#include <vector>
#include <string>
#include <unordered_set>
#include <set>
#include <list>


namespace sat_solver {

    class CNF {
    public:
        friend void dpll(CNF *);
        friend bool dpll_helper(CNF *);
        /// Parses DIMACS CNF formatted file, splitting clause into multiple lines is not supported
        static CNF *parse(const std::string &);
        bool unit_propagate();
        bool pure_literals_eliminate();
        [[nodiscard]] std::unordered_set<int> find_pure_literals() const;
        [[nodiscard]] std::unordered_set<int> find_unit_clauses() const;
        [[nodiscard]] int choose_literal() const;
    private:
        explicit CNF(int vars_num) : vars_num(vars_num) {}
        void eliminate_clauses_with_certain_literals(const std::unordered_set<int> &);
        void eliminate_neg_literals_from_clauses(const std::unordered_set<int> &);
        int vars_num; // is needed to check if num is small enough, then not being touched at all
        bool contains_empty_clause = false;
        std::unordered_set<int> model;
        std::list<std::unordered_set<int>> clauses;
    };

} // sat_solver

#endif //DPLL_CNF_H
