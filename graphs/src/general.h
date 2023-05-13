#pragma once

#include <chrono>
#include <cstdint>
#include <functional>

#define timeit(f, ...) __timeit__((f), #f, __FILE__, __LINE__, ##__VA_ARGS__)

typedef uint32_t city_t;
typedef uint32_t transport_t;

void __timeit_log_call__(const char *name, std::chrono::milliseconds time,
                         const char *file, int line);

template <class F, class... Args>
std::invoke_result_t<F, Args...> __timeit__(F &&f, const char *name,
                                            const char *file, int line,
                                            Args &&...args) {
    if constexpr (std::is_void_v<std::invoke_result_t<F, Args...>>) {
        const auto start = std::chrono::high_resolution_clock::now();
        std::invoke(f, args...);
        const auto end = std::chrono::high_resolution_clock::now();

        __timeit_log_call__(
            name,
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start),
            file, line);
    } else {
        const auto start = std::chrono::high_resolution_clock::now();
        std::invoke_result_t<F, Args...> res = std::invoke(f, args...);
        const auto end = std::chrono::high_resolution_clock::now();

        __timeit_log_call__(
            name,
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start),
            file, line);

        return res;
    }
}
