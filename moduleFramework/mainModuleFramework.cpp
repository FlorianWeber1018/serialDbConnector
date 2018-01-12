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

  SignalRouterOut* modRouterOut =
    new SignalRouterOut("localhost", 3306, "IoD", "637013", "heizung");

  mySqlSignal actualTemp;
  actualTemp.DeviceID = "/dev/ttyACM0";
  actualTemp.PortType = "A";
  actualTemp.Port     = "0";
  actualTemp.Pin      = "0";

  mySqlSignal pwmInc;
  pwmInc.DeviceID = "/dev/ttyACM0";
  pwmInc.PortType = "I";
  pwmInc.Port     = "2";
  pwmInc.Pin      = "5";

  mySqlSignal pwmDec;
  pwmDec.DeviceID = "/dev/ttyACM0";
  pwmDec.PortType = "I";
  pwmDec.Port     = "2";
  pwmDec.Pin      = "6";

  Module_3WayValve* pidMod = new Module_3WayValve();

  Module_debug* modDebugInc = new Module_debug();
  modDebugInc->m_config.identifier = "pwm Inc";

  Module_debug* modDebugDec = new Module_debug();
  modDebugDec->m_config.identifier = "pwm Dec";

  Module_constant* modRequiredTemp = new Module_constant();
  modRequiredTemp->m_config.constValue = 60;


  connect(    //CONNECT DC_INC-> debug
    pidMod,
    pidMod->getSignal("DutyCyclePWMinc"),
    modDebugInc,
    modDebugInc->getSlot("debugSlot")
  );
  connect(    //CONNECT DC_INC-> OUT
    pidMod,
    pidMod->getSignal("DutyCyclePWMinc"),
    modRouterOut,
    pwmInc
  );


  connect(    //CONNECT DC_DEC-> debug
    pidMod,
    pidMod->getSignal("DutyCyclePWMdec"),
    modDebugDec,
    modDebugDec->getSlot("debugSlot")
  );
  connect(    //CONNECT DC_DEC-> OUT
    pidMod,
    pidMod->getSignal("DutyCyclePWMdec"),
    modRouterOut,
    pwmDec
  );

  connect(    //CONNECT Temperature Signal -> actualTemperatur of PID
    modRouterIn,
    actualTemp,
    pidMod,
    pidMod->getSlot("actualTemperature")
  );

  connect(    //CONNECT Temperature Signal -> actualTemperatur of PID
    modRequiredTemp,
    modRequiredTemp->getSignal("constSig"),
    pidMod,
    pidMod->getSlot("requiredTemperature")
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
  Clock clock(std::chrono::milliseconds(200),
  triggerGlobalClock );
  if (debugMode) std::cout << "Now starting mainloop" << std::endl;
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
