#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <math.h>
#include <string>
#include <cstring>
#include <numeric>
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

void Table::parser() {
    size_t N = num_outgoing.size();
    p = std::make_shared<SparseMatrix>(1, N);  // 行数为1，列数为N
    double initialPR = 1.0 / N;

    for (size_t i = 0; i < N; ++i) {
        p->values.push_back(initialPR);  // 填充值
        p->columns.push_back(i);         // 列索引，从0到N-1
    }
    p->rowPointers[0] = 0;  // 第一行开始于索引0
    p->rowPointers[1] = N;  // 第一行结束于索引N


    // std::cout << *p << std::endl;

    // M = std::make_shared<SparseMatrix>(N, N);
    // for (size_t j = 0; j < rows.size(); ++j) {
    //     for (size_t i : rows[j]) {
    //         double probability = 1.0 / num_outgoing[j];
    //         M->values.push_back(probability);
    //         M->columns.push_back(i);
    //         M->rowPointers[j + 1]++;
    //     }
    // }

    // for (size_t i = 1; i <= num_outgoing.size(); ++i) {
    //     M->rowPointers[i] += M->rowPointers[i - 1];
    // } 

    M = std::make_shared<SparseMatrix>(N, N);
    M->rowPointers[0] = 0;

    for (size_t j = 0; j < rows.size(); ++j) {
        if (rows[j].empty()) {
            for (size_t i = 0; i < N; ++i) {
                M->values.push_back(1.0 / N);
                M->columns.push_back(i);
            }
            M->rowPointers[j + 1] = M->rowPointers[j] + N;
        } else {
            for (size_t i : rows[j]) {
                double probability = 1.0 / num_outgoing[j];
                M->values.push_back(probability);
                M->columns.push_back(i);
            }
            M->rowPointers[j + 1] = M->rowPointers[j] + rows[j].size();
        }
    }
    // std::cout << *M << std::endl;
}

void Table::pagerank() {
    size_t N = num_outgoing.size();
    
    for (int i = 0; i < MAX_ITER; ++i) {
        p = std::make_shared<SparseMatrix>(*p * *M);
        double sum = std::accumulate(p->values.begin(), p->values.end(), 0.0);
        if (sum != 0) {  // Avoid division by zero
            for (double &value : p->values) {
                value /= sum;
            }
        }
    }

    std::cout << *p << std::endl;
}