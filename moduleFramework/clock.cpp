#include "clock.h"

Clock::Clock(std::chrono::milliseconds T, void (*callback)())
{
  this->T = T;
  this->m_callback=callback;
}

void Clock::run()
{
  std::this_thread::sleep_for(T);
  if(m_run){
    std::thread t(&Clock::run, this);
    t.detach();
  }
  m_callback();
}

void Clock::stop(){
  bool run=false;
}

void Clock::setTimebase(std::chrono::milliseconds T)
{
  this->T = T;
}
