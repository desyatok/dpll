#include "DPLL.h"
#include <iostream>
#include <algorithm>

using namespace sat_solver;
using std::unordered_set;
using std::vector;

bool
sat_solver::dpll_helper(CNF *formula) {

    while (true) {
        bool units = formula->unit_propagate();
        bool pures = formula->pure_literals_eliminate();
        if (formula->contains_empty_clause) return false;
        if (!units && !pures) break; // no changes were made or empty clause appeared
    }

    if (formula->clauses.empty()) return true;

    int literal = formula->choose_literal();

    CNF *branch = new CNF(*formula);
    unordered_set<int> new_clause = {literal};
    branch->clauses.push_front(new_clause);

    bool solve_with_literal = dpll_helper(branch);
    if (solve_with_literal) {
        formula->model = branch->model;
        delete branch;
        return true;
    }
    delete branch;

    unordered_set<int> new_clause_neg = {-literal};
    formula->clauses.push_front(new_clause_neg);
    return dpll_helper(formula);
}

void
sat_solver::dpll(CNF *formula) {
    if (!dpll_helper(formula)) {
        std::cout << "s UNSATISFIABLE" << std::endl;
    } else {
        for (int literal = 1 ; literal <= formula->vars_num; ++literal) {
            auto literal_in_model = formula->model.find(literal) != formula->model.end() ||
                    formula->model.find(-literal) != formula->model.end();
            if (!literal_in_model) formula->model.insert(literal);
        }

        vector<int> model = {formula->model.begin(), formula->model.end()};
        std::sort(model.begin(), model.end(), [](int num1, int num2){
            return abs(num1) < abs(num2);
        });

        std::cout << "s SATISFIABLE" << std::endl << "v ";
        for (int literal : model) {
            std::cout << literal << ' ';
        }
        std::cout << "0" << std::endl;
    }
}