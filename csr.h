#include <iostream>
#include <vector>
#include <iomanip>

class SparseMatrix {
public:
    std::vector<double> values;
    std::vector<int> columns;
    std::vector<int> rowPointers;
    int numRows;
    int numCols;

    SparseMatrix();
    SparseMatrix(int rows, int cols);
    SparseMatrix operator*(const SparseMatrix& B) const;
    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);
};