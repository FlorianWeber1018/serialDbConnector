#ifndef clock__h
#define clock__h
#include <chrono>
#include <iostream>
#include <thread>

class Clock
{
public:
  Clock(std::chrono::milliseconds T, void (*callback)(void));
  void run();
  void stop();
  void setTimebase(std::chrono::milliseconds T);
protected:
  bool m_run;

  std::chrono::milliseconds T;
  void (*m_callback)(void);
private:

};

#endif
