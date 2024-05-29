#include "Clock.hpp"

Clock::Clock()
{
    restart();
}

void Clock::restart()
{
    start_time = std::chrono::high_resolution_clock::now();
}

float Clock::getElapsedTime() const
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed_time = current_time - start_time;
    return elapsed_time.count();
}
