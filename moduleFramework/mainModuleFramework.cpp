#include "mainModuleFramework.h"
#include "clock.h"
ClockDistributer globalClock;

int main()
{
  /*Module* _1x1 = new Module();
  _1x1->*/
  Module_constant modConst;
  modConst.m_config.constValue = 42;


  Module_debug modDebug;
  modDebug.m_config.identifier = "test passed when there is the truth ";

  connect(
    static_cast<Module*>(&modConst),
    modConst.getSignal("constSig"),
    static_cast<Module*>(&modDebug),
    modDebug.getSlot("debugSlot")
  );
  Clock clock(std::chrono::milliseconds(1000));
  clock.run();
  //globalClock.trigger();
  return 0;
}
