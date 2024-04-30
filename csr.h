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
    SparseMatrix operator+(const SparseMatrix& B) const;
    friend SparseMatrix operator*(const SparseMatrix& A, double scalar);
    friend SparseMatrix operator*(double scalar, const SparseMatrix& A);

    friend std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix);
};