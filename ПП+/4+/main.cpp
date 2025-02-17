#include <omp.h>
#include <cmath>
#include <iostream>
#include <cstdlib> // Для функции atoi

static constexpr double f(double a) {
    double c = 0.8;
    return sin(c * a) * cos(c * a);
}

static constexpr double fi(double a) {
    double c = 0.8;
    return (1 / (2 * c)) * pow(sin(c * a), 2);
}

int main(int argc, char *argv[]) {
    int n = 100000000; // Количество шагов для интегрирования
    double xl = -0.5, xh = 0.8; // Границы интегрирования
    double h = (xh - xl) / (double)n; // Шаг интегрирования
    double sum = 0.0;

    int num_threads = 4; // Число потоков по умолчанию

    // Если указано число потоков через аргументы командной строки
    if (argc > 1) {
        num_threads = atoi(argv[1]); // Чтение количества потоков
    }

    omp_set_num_threads(num_threads); // Устанавливаем число потоков

    double start_time = omp_get_wtime(); // Время начала

    // Параллельный регион с директивой for и редукцией
    #pragma omp parallel//Создаём паралеллельный регион
    {
        #pragma omp single// Одним потоком выполняем вывод текста
        {
            std::cout << "С использованием " << omp_get_num_threads() << " потоков." << std::endl;
        }

        #pragma omp for reduction(+:sum) schedule(static, 1000) // распределение итераций цикла по потокам(статическим способо, 1000 в каждый)
        for (int i = 1; i <= n; i++) {
            double x = xl + h * (i - 0.5);
            sum += f(x);
        }
    }

    double result = h * sum; // Итоговая сумма
    double end_time = omp_get_wtime(); // Время завершения

    // Вывод результатов в мастер-потоке
    std::cout << "Максимальное количество доступных потоков: " << omp_get_max_threads() << std::endl;
    std::cout << "Результат интегрирования: " << result << std::endl;
    std::cout << "Ошибка: " << result - (fi(xh) - fi(xl)) << std::endl;
    std::cout << "Потраченое время: " << end_time - start_time << " seconds." << std::endl;

    return 0;
}
