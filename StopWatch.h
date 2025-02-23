#include <iostream>
#include <chrono>

class Stopwatch {
public:
    Stopwatch(){
        start_time = std::chrono::high_resolution_clock::now();
    }

    void restart(){
        start_time = std::chrono::high_resolution_clock::now();
    }

    // Останавливает отсчет времени и возвращает продолжительность
    double stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end_time - start_time;
        return elapsed.count(); // Возвращает время в миллисекундах
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};
