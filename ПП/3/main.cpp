#include <mpi.h>
#include <iostream>
#include <vector>

constexpr int N = 5;  // Размер матрицы N x N

// Печать матрицы
void print_matrix(const int* matrix, int rows, int cols, bool trim = false) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if ((j < rows-i) || !trim) {
                std::cout << matrix[i * cols + j] << " ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int matrix[N * N];  // Исходная матрица
    int recv_matrix[N * N] = {0};  // Матрица для приема данных
    int T=(N*(N+1))/2;
    int select_vector[T]= {0};

    if (rank == 0) {
        // Заполняем исходную матрицу числами от 1 до N*N
        for (int i = 0; i < N * N; ++i) {
            matrix[i] = i + 1;
        }

        std::cout << "Процесс 0: Оригинальная матрица:" << std::endl;
        print_matrix(matrix, N, N);
    }

    // Определяем массивы смещений и количества элементов для индексного типа
    std::vector<int> block_lengths(N);
    std::vector<int> displacements(N);

    // Для левого верхнего треугольника
    for (int i = 0; i < N; ++i) {
        block_lengths[i] = N - i;  // Количество элементов в строке уменьшается
        displacements[i] = i * N;  // Смещение начала строки
    }

    // Создание производного типа данных (индексного)
    MPI_Datatype triangle_type;
    MPI_Type_indexed(N, block_lengths.data(), displacements.data(), MPI_INT, &triangle_type);
    MPI_Type_commit(&triangle_type);
    
    if (rank == 0) {
        // Отправка данных во второй процесс (процесс 1)
        MPI_Send(matrix, 1, triangle_type, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        // Прием данных в процесс 1
        MPI_Recv(recv_matrix, 1, triangle_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(recv_matrix, N*N, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }
    else if (rank ==2){
        // Прием данных в процесс 2
        MPI_Recv(recv_matrix, N*N, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Процесс 2: Левый треугольник матрицы:" << std::endl;
        print_matrix(recv_matrix, N, N, true);

        std::cout << "Процесс 2: Выбраный вектор:" << std::endl;
        
        int itr=0;
        int tr=N;
        for (int i = 0; i < 5; ++i)
        {
            for (int j = 0; j <tr; ++j)
            {
                select_vector[itr] = recv_matrix[i * 5 + j];
                itr++;
            }
            tr--;
        }
        std::cout << std::endl;
    }

    if (rank == 2) {
        // Отправка данных во второй процесс (процесс 1)
        MPI_Send(select_vector, T, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else if (rank == 0) {
        // Прием данных в процесс 1
        MPI_Recv(recv_matrix, 1, triangle_type, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Процесс 0: Другая матрица:" << std::endl;
        print_matrix(recv_matrix, N, N, true);
    }



    // Освобождение производного типа данных
    MPI_Type_free(&triangle_type);

    MPI_Finalize();
    return 0;
}
