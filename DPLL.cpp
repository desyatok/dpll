#include "DPLL.h"

using namespace sat_solver;
using std::unordered_set;
using std::vector;

bool
sat_solver::dpll(CNF *formula, std::vector<int> &model) {
    if (formula->clauses_num == 0) return true;
    if (formula->contains_empty_clause) return false;
    while (true) {
        bool units = formula->unit_propagate();
        bool pures = formula->pure_literals_eliminate();
        if (!units && !pures || formula->contains_empty_clause) break; // no changes were made or empty clause appeared
    }

    int literal = formula->choose_literal();

    CNF *branch = new CNF(*formula);
    unordered_set<int> new_clause = {literal};
    branch->clauses.push_back(new_clause);

    auto model_size = model.size();
    bool solve_with_literal = dpll(branch,model);
    if (solve_with_literal) {
        delete branch;
        return true;
    }

    model.erase(model.cbegin() + model_size, model.cend());

    unordered_set<int> new_clause_neg = {-literal};
    formula->clauses.push_back(new_clause_neg);
    return dpll(formula,model);
}
