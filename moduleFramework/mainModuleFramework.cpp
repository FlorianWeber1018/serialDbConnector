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
  SignalRouterIn* modRouterIn =
    new SignalRouterIn("localhost", 3306, "IoD", "637013", "heizung");

  mySqlSignal temp;
  temp.DeviceID = "/dev/ttyACM0";
  temp.PortType = "A";
  temp.Port     = "0";
  temp.Pin      = "0";




  Module_debug* modDebug = new Module_debug();
  modDebug->m_config.identifier = "TEST";

  connect(
    modRouterIn,
    temp,
    modDebug,
    modDebug->getSlot("debugSlot")
  );


/*
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
*/




  initGlobalClock();

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
  if (debugMode) std::cout << "Now starting mainloop"
  clock.run();
  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }
}

/*void test(int in)
{
  ServoPWM dut;
  int inc;
  int dec;
  dut.getOutput(inc, dec, in);
  std::cout << "input = " << in << "  incPWM = " << inc <<
  "  decPWM = " << dec << std::endl;
}
*/
