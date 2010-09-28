#ifndef INCLUDED_TIMER_H
#define INCLUDED_TIMER_H

#include <ctime>

class Timer
{
public:
  Timer() : start_(std::clock()) {}

  std::clock_t elapsed() const {
    return clock() - start_;
  }
  unsigned elapsed_sec() const {
    return elapsed() / CLOCKS_PER_SEC;
  }

  void reset() { start_ = std::clock(); }
  
private:
  std::clock_t start_;
};

#endif // INCLUDED_TIMER_H
