#include <mpi.h>
#include <math.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <ctime>

// Вариант 9: вычислить поэлементное сложение элементов двух матриц Y = X + Y

constexpr uint8_t rows = 5;
constexpr uint8_t cols = 20;

// Инициализация матрицы случайными числами
void initialize_matrix(double* matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = rand() % 100;
    }
}

// Печать матрицы
void print_matrix(const double* matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    int numprocs, myid, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    const int matrix_size = rows * cols; // Размер матрицы

    double* X = nullptr; // Матрица X
    double* Y = nullptr; // Матрица Y

    // Порция матриц для каждого процесса
    int portion_size = matrix_size / numprocs;
    double* local_X = new double[portion_size];
    double* local_Y = new double[portion_size];

    if (myid == 0) {
        // Инициализация матриц на главном процессе
        X = new double[matrix_size];
        Y = new double[matrix_size];
        initialize_matrix(X, rows, cols);
        initialize_matrix(Y, rows, cols);

        std::cout << "Матрица X:" << std::endl;
        print_matrix(X, rows, cols);

        std::cout << "матрица Y:" << std::endl;
        print_matrix(Y, rows, cols);
    }

    // Рассылка порций матриц X и Y по процессам
    MPI_Scatter(X, portion_size, MPI_DOUBLE, local_X, portion_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Y, portion_size, MPI_DOUBLE, local_Y, portion_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Поэлементное сложение локальных частей
    for (int i = 0; i < portion_size; ++i) {
        local_Y[i] += local_X[i];
    }

    // Сбор результатов в главном процессе
    MPI_Gather(local_Y, portion_size, MPI_DOUBLE, Y, portion_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Вывод результата на главном процессе
    if (myid == 0) {
        std::cout << "Результат Y = X + Y:" << std::endl;
        print_matrix(Y, rows, cols);
    }

    // Освобождение памяти
    delete[] local_X;
    delete[] local_Y;
    if (myid == 0) {
        delete[] X;
        delete[] Y;
    }

    MPI_Finalize();
    return 0;
}

