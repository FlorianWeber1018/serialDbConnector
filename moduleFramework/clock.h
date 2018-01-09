#ifndef clock__h
#define clock__h
#include <crono>
#include <iostream>
#include <thread>

class Clock
{
public:
  Clock(std::crono::milliseconds T);
  void run(bool runContiniusly);
  void stop();
  void setTimebase(std::crono::milliseconds T);
protected:
  bool run;
  void tick();
  std::crono::milliseconds T
private:

};

#endif
