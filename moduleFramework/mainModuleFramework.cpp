#include "mainModuleFramework.h"
#include "clock.h"
#include <thread>
ClockDistributer globalClock;


int main()
{
  /*Module* _1x1 = new Module();
  _1x1->*/
  Module_constant modConst;
  modConst.m_config.constValue = 50;


  Module_debug modDebug;
  modDebug.m_config.identifier = "test passed when there is the truth ";

  connect(
    static_cast<Module*>(&modConst),
    modConst.getSignal("constSig"),
    static_cast<Module*>(&modDebug),
    modDebug.getSlot("debugSlot")
  );



  init();
  //globalClock.trigger();
  return 0;
}

void triggerGlobalClock()
{
  globalClock.trigger();
}
void init()
{
  Clock clock(std::chrono::milliseconds(100),
  triggerGlobalClock );
  clock.run();
  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}
