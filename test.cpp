#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

void matrix_multiply(const vector<vector<double>> &A, 
    const vector<vector<double>> &B, vector<vector<double>> &C) {
        int n = A.size();
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k < n; ++k) {
                for (int j = 0; j < n; ++j) {
                    C[i][k] += A[i][j] * B[j][k];
                }
            }
        }
}

void matrix_multiply_optimized(const vector<vector<double>> &A, 
    const vector<vector<double>> &B, vector<vector<double>> &C) {
        int n = A.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int temp = A[i][j];
                for (int k = 0; k < n; ++k) {
                    C[i][j] += temp * B[j][k];
                }
            }
        }
}
     
void matrix_multiply_unrolled(const vector<vector<double>> &A, 
    const vector<vector<double>> &B, vector<vector<double>> &C) {
        int n = A.size();
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; j += 4) {
                int temp = A[i][j];
                for (int k = 0; k < n; ++k) {
                    C[i][j] += temp * B[j][k];
                    C[i][j + 1] += temp * B[j + 1][k];
                    C[i][j + 2] += temp * B[j + 2][k];
                    C[i][j + 3] += temp * B[j + 3][k];
                }
            }
        }
}

int main() {
    int n = 512;
    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> B(n, vector<double>(n));
    vector<vector<double>> C(n, vector<double>(n));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 100);

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            A[i][j] = dist(gen);
            B[i][j] = dist(gen);
        }
    }

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            C[i][j] = 0;
        }
    }

    auto start = chrono::high_resolution_clock::now();
    matrix_multiply(A, B, C);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            C[i][j] = 0;
        }
    }

    auto start1 = chrono::high_resolution_clock::now();
    matrix_multiply_optimized(A, B, C);
    auto end1 = chrono::high_resolution_clock::now();
    auto duration_optimized = chrono::duration_cast<chrono::milliseconds>(end1 - start1);

    for (int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            C[i][j] = 0;
        }
    }

    auto start2 = chrono::high_resolution_clock::now();
    matrix_multiply_unrolled(A, B, C);
    auto end2 = chrono::high_resolution_clock::now();
    auto duration_unrolled = chrono::duration_cast<chrono::milliseconds>(end2 - start2);

    cout << "C++ time: " << duration.count() << " ms" << endl;
    cout << "C++ optimized time: " << duration_optimized.count() << " ms" << endl;
    cout << "C++ unrolled time: " << duration_unrolled.count() << " ms" << endl;

    return 0;
}
