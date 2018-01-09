#include "clock.h"

Clock::Clock(std::chrono::milliseconds T)
{
  this->T = T;
}

void Clock::run()
{
  std::this_thread::sleep_for(T);
  if(m_run){
    std::thread t(&run);
  }
  std::cout << "TICK!" << std::endl;
}

void Clock::stop(){
  bool run=false;
}

void Clock::setTimebase(std::chrono::milliseconds T)
{
  this->T = T;
}

void Clock::tick()
{



}



while (run) {

}
