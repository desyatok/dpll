#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>

using std::vector;
using std::string;
using std::unordered_set;
constexpr size_t MAX_VARS_NUMBER = 1500;

class CNF {
    int vars_num;
    int clauses_num;
    vector<vector<int>> clauses;
    CNF(int vars_num, int clauses_num) : vars_num(vars_num), clauses_num(clauses_num), clauses() {}

public:
    CNF() = delete;

    /// Parses DIMACS CNF formatted file, splitting clause into multiple lines is not supported
    static CNF *parse(const string &input) {
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
            vector<int> clause;
            unordered_set<int> literals_in_use;

            std::getline(file,line);
            std::istringstream line_stream(line);
            line_stream >> literal;

            // If literal has 2 or more occurrences or has its negative value at the same clause,
            // first occurrence is written and others are ignored. That is why abs() is used
            while (literal) {
                if (literals_in_use.find(abs(literal)) != literals_in_use.end()) {
                    line_stream >> literal;
                    continue;
                }
                literals_in_use.insert(abs(literal));
                clause.push_back(literal);
                line_stream >> literal;
            }
            formula->clauses.push_back(clause);
        }
        return formula;
    }
};


int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
