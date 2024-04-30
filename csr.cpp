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

SparseMatrix operator*(const SparseMatrix& A, double scalar) {
    SparseMatrix result(A.numRows, A.numCols);
    result.rowPointers = A.rowPointers; // 直接复制行指针

    for (size_t i = 0; i < A.values.size(); i++) {
        result.values.push_back(A.values[i] * scalar);
        result.columns.push_back(A.columns[i]);
    }

    return result;
}

SparseMatrix operator*(double scalar, const SparseMatrix& A) {
    return A * scalar; // 利用交换律，直接调用前一个函数
}

SparseMatrix SparseMatrix::operator+(const SparseMatrix& B) const {
    if (numRows != B.numRows || numCols != B.numCols) {
        throw std::invalid_argument("Matrices dimensions must be the same for addition.");
    }

    SparseMatrix C(numRows, numCols);
    for (int i = 0; i < numRows; ++i) {
        int a_index = rowPointers[i];
        int b_index = B.rowPointers[i];

        while (a_index < rowPointers[i + 1] && b_index < B.rowPointers[i + 1]) {
            int a_col = columns[a_index];
            int b_col = B.columns[b_index];

            if (a_col == b_col) {
                double sum = values[a_index] + B.values[b_index];
                if (sum != 0.0) {
                    C.values.push_back(sum);
                    C.columns.push_back(a_col);
                }
                a_index++;
                b_index++;
            } else if (a_col < b_col) {
                C.values.push_back(values[a_index]);
                C.columns.push_back(a_col);
                a_index++;
            } else {
                C.values.push_back(B.values[b_index]);
                C.columns.push_back(b_col);
                b_index++;
            }
        }

        while (a_index < rowPointers[i + 1]) {
            C.values.push_back(values[a_index]);
            C.columns.push_back(columns[a_index]);
            a_index++;
        }

        while (b_index < B.rowPointers[i + 1]) {
            C.values.push_back(B.values[b_index]);
            C.columns.push_back(B.columns[b_index]);
            b_index++;
        }

        C.rowPointers[i + 1] = C.values.size();
    }

    return C;
}

// int main() {
//     test1();
//     test2();

//     return 0;
// }

void test1() {
    SparseMatrix A(2, 3);
    A.values = {0.3, 0.5, 0.4};
    A.columns = {0, 2, 1};
    A.rowPointers = {0, 2, 3};

    std::cout << A << std::endl;

    SparseMatrix B = 2.0 * A;

    std::cout << B << std::endl;
}

void test2() {
    int n = 4;
    SparseMatrix matrix(n, n);

    // Initialize all elements to 1
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix.values.push_back(1.0);
            matrix.columns.push_back(j);
        }
        matrix.rowPointers[i] = i * n;
    }
    matrix.rowPointers[n] = n * n;

    SparseMatrix A = matrix * 2.1 + matrix;
    SparseMatrix B = matrix + A;
    std::cout << B  << std::endl;
}