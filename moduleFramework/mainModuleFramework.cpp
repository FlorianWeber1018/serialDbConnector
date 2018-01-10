#include "mainModuleFramework.h"
#include "clock.h"
#include <thread>
#include "control.h"
#include <iostream>

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


  test(30001);
  test(30000);
  test(29999);
  test(5883);
  test(5882);
  test(5881);
  test(0);
  test(-5881);
  test(-5882);
  test(-5883);
  test(-29999);
  test(-30000);
  test(-30001);





  //init();

  return 0;
}

void triggerGlobalClock()
{
  globalClock.trigger();
}
void initGlobalClock()
{
  Clock clock(std::chrono::milliseconds(100),
  triggerGlobalClock );
  clock.run();
  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}

void test(int in)
{
  ServoPWM dut;
  int inc;
  int dec;
  dut.getOutput(inc, dec, in);
  std::cout << "input = " << in << "  incPWM = " << inc <<
  "  decPWM = " << dec << std::endl;
}
