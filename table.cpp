#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <math.h>
#include <string>
#include <cstring>
#include <limits>

#include "table.h"

int Table::read_file(const string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string from, to;
        ss >> from >> to;
        add_arc(std::stoi(from), std::stoi(to));
    }

    return 0;
}

bool Table::add_arc(size_t from, size_t to) {
    rows_resize(std::max(from, to));
    
    vector<size_t> &row = rows[from];
    row.push_back(to);
    num_outgoing[from]++;

    return true;
}

int Table::rows_resize(size_t dim) {
    if (rows.size() <= dim) {
        rows.resize(dim + 1); // index begin at 0
        pr.resize(dim + 1);
        num_outgoing.resize(dim + 1);
    }
    return rows.size();
}

void Table::pagerank() {
    vector<double> old_pr;
    double sum_pr;
    double dangling_pr;
    double diff = 1;
    unsigned long num_itertations = 0;

    size_t n = rows.size();
    for (size_t i = 0; i < n; i++) {
        pr[i] = 1.0 / n;
    }
    old_pr = pr;

    while (diff > EPSILON && num_itertations < MAX_ITER) {
        sum_pr = 0;
        dangling_pr = 0;

        for (size_t i = 0; i < n; i++) {
            double cpr = pr[i];
            sum_pr += cpr;
            if (num_outgoing[i] == 0) {
                dangling_pr += cpr;
            }
        }

        for (int i = 0; i < n; i++) {
            old_pr[i] = pr[i] / sum_pr;
        }

        sum_pr = 1;
        /* An element of the A x I vector; all elements are identical */
        double one_Av = D * dangling_pr / n;

        /* An element of the 1 x I vector; all elements are identical */
        double one_Iv = (1 - D) * sum_pr / n;

        for (size_t i = 0; i < n; i++) {
            double h = 0.0;
            for (size_t j = 0; j < rows[i].size(); j++) {
                size_t& ci = rows[i][j]; /* The current element of the H vector */
                /* The current element of the H vector */
                double h_v = (num_outgoing[ci])
                    ? 1.0 / num_outgoing[ci]
                    : 0.0;
                h += h_v * old_pr[ci];
            }
            h *= D;
            pr[i] = h + one_Av + one_Iv;
        }

        diff = 0;
        for (size_t i = 0; i < n; i++) {
            diff += fabs(pr[i] - old_pr[i]);
        }

        

        num_itertations++;
    }

}