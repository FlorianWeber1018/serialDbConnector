#ifndef clock__h
#define clock__h
#include <chrono>
#include <iostream>
#include <thread>

class Clock
{
public:
  Clock(std::chrono::milliseconds T);
  void run();
  void stop();
  void setTimebase(std::chrono::milliseconds T);
protected:
  bool m_run;
  void tick();
  std::chrono::milliseconds T
private:

};

#endif
