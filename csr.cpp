#include "csr.h"

SparseMatrix::SparseMatrix(): numRows(0), numCols(0) {}

SparseMatrix::SparseMatrix(int rows, int cols) : numRows(rows), numCols(cols) {
    rowPointers.resize(numRows + 1, 0);
}

SparseMatrix SparseMatrix::operator*(const SparseMatrix& B) const {
    if (numCols != B.numRows) {
        throw std::invalid_argument("Matrix dimensions must be compatible for multiplication.");
    }

    SparseMatrix C(numRows, B.numCols);

    std::vector<int> column_cache(B.numCols, -1);
    std::vector<double> temp_row(B.numCols, 0.0); 

    int row_start_A, row_end_A, row_start_B, row_end_B;
    for (int i = 0; i < numRows; ++i) {
        row_start_A = rowPointers[i];
        row_end_A = rowPointers[i + 1];

        for (int j = row_start_A; j < row_end_A; ++j) {
            int A_col = columns[j];
            double A_val = values[j];  

            row_start_B = B.rowPointers[A_col];
            row_end_B = B.rowPointers[A_col + 1];

            for (int k = row_start_B; k < row_end_B; ++k) {
                int B_col = B.columns[k];
                double B_val = B.values[k]; 

                if (column_cache[B_col] != i) {
                    column_cache[B_col] = i;
                    temp_row[B_col] = 0.0;
                }
                temp_row[B_col] += A_val * B_val;
            }
        }

        for (int col = 0; col < B.numCols; ++col) {
            if (column_cache[col] == i) {
                C.values.push_back(temp_row[col]);
                C.columns.push_back(col);
            }
        }
        C.rowPointers[i + 1] = C.values.size();
    }

    return C;
}

std::ostream& operator<<(std::ostream& os, const SparseMatrix& matrix) {
    // 创建一个二维矩阵，所有值初始化为0 
    std::vector<std::vector<double>> fullMatrix(matrix.numRows, std::vector<double>(matrix.numCols, 0.0));

    // 填充非零值
    for (int i = 0; i < matrix.numRows; ++i) {
        for (int idx = matrix.rowPointers[i]; idx < matrix.rowPointers[i + 1]; ++idx) {
            fullMatrix[i][matrix.columns[idx]] = matrix.values[idx];
        }
    }

    // 打印矩阵
    for (const auto& row : fullMatrix) {
        for (const auto& elem : row) {
            os << std::setw(12) << elem; // 宽度设置为4
        }
        os << std::endl;
    }
    return os;
}


// int main() {
//     SparseMatrix A(2, 3);
//     A.values = {0.3, 0.5, 0.4};
//     A.columns = {0, 2, 1};
//     A.rowPointers = {0, 2, 3};

//     SparseMatrix B(3, 3);
//     B.values = {0.2, 0.8, 0.7};
//     B.columns = {1, 0, 2};
//     B.rowPointers = {0, 1, 1, 3};

//     SparseMatrix C = A * B;

//     std::cout << A << std::endl;

//     std::cout << B << std::endl;

//     std::cout << C << std::endl;

//     return 0;
// }
