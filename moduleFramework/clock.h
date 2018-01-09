#ifndef clock__h
#define clock__h
#include <crono>
#include <iostream>
#include <thread>

class Clock
{
public:
  Clock(std::crono::milliseconds T);
  void run();
  void stop();
  void setTimebase(std::crono::milliseconds T);
protected:
  bool m_run;
  void tick();
  std::crono::milliseconds T
private:

};

#endif
