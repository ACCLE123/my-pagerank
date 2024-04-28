#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <map>
using std::vector;
using std::set;
using std::map;
using std::string;

const double D = 0.85;
const double EPSILON = 0.00001; // convergence check
const int MAX_ITER = 10000; // max iteration
const bool NUMERIC = true; // if true, use numeric pagerank, else use algebraic pagerank
const std::string DELIM = " ";

class Table {
private:
    std::vector<double> pr;
    std::vector<size_t> num_outgoing;
    vector<vector<size_t>> rows;
public:
    int read_file(const string &filename);
    void pagerank();
    bool add_arc(size_t from, size_t to);
    int rows_resize(size_t dim);

    void test() {
        std::cout << "test" << std::endl;

        for (auto x : pr)
            std::cout << x << ' ';
        std::cout << std::endl;
    }
};