// Вариант 9 - Производитель – потребитель. Одиночный буфер. Замок и условная переменная (п.2.3).

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <omp.h>
#include <queue>
#include <cstdlib>
#include <chrono>
#include <concepts>

constexpr auto MAX_ITERATIONS = 10;

inline static auto random_sleep(int min, int max) -> void {
    std::this_thread::sleep_for(std::chrono::milliseconds(min + rand() % (max - min + 1)));
}

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

struct [[nodiscard]] BufferState {
    std::queue<decltype(int())> queue;
    std::atomic_bool buffer_full = false;
};

std::mutex mtx;
std::condition_variable cv;
std::atomic_bool done = false;

namespace mp_sections {
inline auto producer(Numeric auto max_iterations, BufferState& state) -> void {
    for (auto i = 1; i <= max_iterations; ++i) {
        random_sleep(1, 100);
        std::unique_lock lock(mtx);
        cv.wait(lock, [&state] { return !state.buffer_full; });

        auto value = i;  // Выработка значения
        state.queue.push(value);
        std::cout << "\033[35m-> Producer\033[0m: " << value << " at iteration " << i << "\n";
        state.buffer_full = true;
        cv.notify_one();
    }

    done = true;
    cv.notify_all();
}

inline auto consumer(Numeric auto max_iterations, BufferState& state) -> void {
    for (auto i = 1; i <= max_iterations; ++i) {
        std::unique_lock lock(mtx);
        cv.wait(lock, [&state] { return state.buffer_full || done; });

        if (!state.queue.empty()) {
            auto value = state.queue.front();
            state.queue.pop();
            std::cout << "\033[36m<- Consumer\033[0m: " << value << " at iteration " << i << "\n";
            state.buffer_full = false;
            cv.notify_one();
        }
    }
}
} // namespace mp_sections

// Основная функция
auto main(int argc, char** argv) -> decltype(int()) {
    auto iterations = MAX_ITERATIONS;
    if (argc > 1) {
        iterations = std::atoi(argv[1]);
    }
    BufferState buffer_state{};
    // Параллелизация через секции
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mp_sections::producer(iterations, buffer_state);
        }
        #pragma omp section
        {
            mp_sections::consumer(iterations, buffer_state);
        }
    }
    return {};
}
