#include "CNF.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

constexpr auto MAX_VARS_NUMBER = 1500;
constexpr auto tooManyVars = "Too many Variables are given";
constexpr auto notDimacs = "Not a DIMACS CNF format";


using namespace sat_solver;
using std::vector;
using std::string;
using std::unordered_set;

CNF *
CNF::parse(const std::string &input) {
    std::ifstream file(input);
    if (!file) throw std::runtime_error("Could not open a file");
    string line, tmp_line;
    int vars_number, clauses_number;
    CNF *formula;

    while (true) {
        std::getline(file,line);
        if (line[0] == 'c') continue;
        if (line[0] == 'p') {
            std::istringstream line_stream(line);

            line_stream >> tmp_line >> tmp_line >> vars_number >> clauses_number;

            if (vars_number < 1 || clauses_number < 1) throw std::invalid_argument(notDimacs);

            formula = new CNF(vars_number, clauses_number);

            if (formula->vars_num > MAX_VARS_NUMBER) {
                delete formula;
                throw std::invalid_argument(tooManyVars);
            }
            break;
        }
        throw std::invalid_argument(notDimacs);
    }

    for (size_t i = 0; i < clauses_number; ++i) {
        int literal;
        unordered_set<int> clause;
        bool meaningless_clause = false;

        std::getline(file,line);
        std::istringstream line_stream(line);
        line_stream >> literal;
        if (abs(literal) > formula->vars_num) throw std::invalid_argument(notDimacs);

        while (literal) {
            if (clause.find(-literal) != clause.end()) { // if -literal and literal are in the same clause,
                                                            // clause is not inserted into a formula
                meaningless_clause = true;
                formula->clauses_num--;
                break;
            }
            clause.insert(literal);
            line_stream >> literal;
            if (abs(literal) > formula->vars_num) throw std::invalid_argument(notDimacs);
        }
        if (!meaningless_clause) formula->clauses.push_back(clause);
    }
    return formula;
}

void
CNF::eliminate_clauses_with_certain_literals(const std::unordered_set<int> &literals) {
    for (auto literal : literals) {
        auto to_be_removed = std::remove_if(clauses.begin(), clauses.end(),
                                            [literal](const unordered_set<int> &clause) {
                                                return clause.find(literal) != clause.end();
        });
        clauses_num -= clauses.end() - to_be_removed;
        clauses.erase(to_be_removed, clauses.end());
    }
}

void
CNF::eliminate_neg_literals_from_clauses(const std::unordered_set<int> &literals) {
    for (auto &clause : clauses) {
        for (auto literal : literals) {
            auto reverse_polarity = clause.find(-literal);
            if (reverse_polarity != clause.end()) {
                clause.erase(reverse_polarity);
                if (clause.empty()) contains_empty_clause = true;
            }
        }
    }
}

bool
CNF::pure_literals_eliminate() {
    unordered_set<int> pures = find_pure_literals();
    if (pures.empty()) return false;
    model.insert(pures.begin(), pures.end());
    eliminate_clauses_with_certain_literals(pures);
    return true;
}

unordered_set<int>
CNF::find_pure_literals() const {
    unordered_set<int> not_pures;
    unordered_set<int> pures;

    for (const auto &clause : clauses) {
        for (auto literal : clause) {
            if (not_pures.find(abs(literal)) != not_pures.end()) continue;

            auto reverse_polarity = pures.find(-literal);
            if (reverse_polarity != pures.end()) {
                not_pures.insert(abs(*reverse_polarity));
                pures.erase(reverse_polarity);
            } else if (pures.find(literal) == pures.end()) {
                pures.insert(literal);
            }
        }
    }
    return pures;
}

bool
CNF::unit_propagate() {
    unordered_set<int> unit_clauses = find_unit_clauses();
    if (unit_clauses.empty()) return false;

    for (auto literal : unit_clauses) {
        if (unit_clauses.find(-literal) != unit_clauses.end()) {
            contains_empty_clause = true;
            return false;
        }
    }

    model.insert(unit_clauses.begin(), unit_clauses.end());
    eliminate_clauses_with_certain_literals(unit_clauses);
    eliminate_neg_literals_from_clauses(unit_clauses);
    return true;
}

unordered_set<int>
CNF::find_unit_clauses() const {
    unordered_set<int> unit_clauses;
    for (const auto &clause : clauses) {
        if (clause.size() == 1) {
            unit_clauses.insert(*clause.begin());
        }
    }
    return unit_clauses;
}

int
CNF::choose_literal() const {
    return *clauses[0].begin();
}
