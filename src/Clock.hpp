#include <chrono>

class Clock {
public:

    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;

    Clock();

    void restart();

    float getElapsedTime() const;

private:
    time_point start_time;
};