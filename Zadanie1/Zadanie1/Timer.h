#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer() : m_begin(m_clock::now()) {}

	void reset() { m_begin = m_clock::now(); }
	double elapsed() const {
		return std::chrono::duration_cast<m_seconds>(m_clock::now() - m_begin).count();
	}

private:
	using m_clock = std::chrono::high_resolution_clock;
	using m_seconds = std::chrono::duration<double, std::ratio<1> >;
	std::chrono::time_point<m_clock> m_begin;
};

#endif // !TIMER_H

