#include "clock.h"

Clock::Clock(std::crono::milliseconds T)
{
  this->T = T;
}

void Clock::run(bool runContiniusly)
{
  if(runContiniusly){
    thread t(&Clock::tick, this);
  	t.detach();
  }
  std::this_thread::sleep_for(T);
  std::cout << "TICK!" << std::endl;

}

void Clock::stop(){
  bool run=false;
}

void Clock::setTimebase(std::crono::milliseconds T)
{
  this->T = T;
}

void Clock::tick()
{


}



while (run) {

}
