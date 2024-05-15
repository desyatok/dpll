#include "CNF.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

constexpr size_t MAX_VARS_NUMBER = 1500;

using namespace sat_solver;
using std::vector;
using std::string;
using std::unordered_set;

CNF *
CNF::parse(const std::string &input) {
    std::ifstream file(input);
    string line, tmp_line;
    CNF *formula;

    while (true) {
        std::getline(file,line);
        if (line[0] == 'c') continue;
        if (line[0] == 'p') {
            std::istringstream line_stream(line);
            int vars_number, clauses_number;

            line_stream >> tmp_line >> tmp_line >> vars_number >> clauses_number;
            formula = new CNF(vars_number, clauses_number);

            if (formula->vars_num > MAX_VARS_NUMBER) {
                std::cout << "Too many variables are given" << std::endl;
                delete formula;
                return nullptr;
            }
            break;
        }
        std::cout << "Unknown symbol for a DIMACS CNF format is detected" << std::endl;
        return nullptr;
    }

    for (size_t i = 0; i < formula->clauses_num; ++i) {
        int literal;
        unordered_set<int> clause;
        bool meaningless_clause = false;

        std::getline(file,line);
        std::istringstream line_stream(line);
        line_stream >> literal;

        while (literal) {
            if (clause.find(-literal) != clause.end()) { // if -literal and literal are in the same clause,
                                                            // clause is not inserted into a formula
                meaningless_clause = true;
                formula->clauses_num--;
                break;
            }
            clause.insert(literal);
            line_stream >> literal;
        }
        if (!meaningless_clause) formula->clauses.push_back(clause);
    }
    return formula;
}

void
CNF::eliminate_literals(const unordered_set<int> &literals) {
    for (auto literal : literals) {
        auto to_be_removed = std::remove_if(clauses.begin(), clauses.end(),
                                            [literal](const unordered_set<int> &clause) {
                                                return clause.find(literal) != clause.end();
        });
        clauses_num -= clauses.end() - to_be_removed;
        clauses.erase(to_be_removed, clauses.end());
    }
    for (auto &clause : clauses) {
        for (auto literal : literals) {
            auto reverse_polarity = clause.find(-literal);
            if (reverse_polarity != clause.end()) {
                clause.erase(reverse_polarity);
            }
        }
    }
}

bool
CNF::pure_literals_eliminate() {
    unordered_set<int> pures = find_pure_literals();
    if (pures.empty()) return false;
    eliminate_literals(pures);
    return true;
}

unordered_set<int>
CNF::find_pure_literals() const {
    unordered_set<int> not_pures;
    unordered_set<int> pures;

    for (const auto &clause : clauses) {
        for (auto literal : clause) {
            if (not_pures.find(abs(literal)) == not_pures.end()) continue;

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
    eliminate_literals(unit_clauses);
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
