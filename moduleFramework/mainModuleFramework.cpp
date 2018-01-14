#include "mainModuleFramework.h"
#include "clock.h"
#include <thread>
#include "control.h"
#include <iostream>

ClockDistributer globalClock;
volatile bool recourcesAvailable = true;

int main()
{
  std::this_thread::sleep_for(std::chrono::seconds(15));
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
/*
  Module_debug* modDebugInc = new Module_debug();
  modDebugInc->m_config.identifier = "pwm Inc";

  Module_debug* modDebugDec = new Module_debug();
  modDebugDec->m_config.identifier = "pwm Dec";
*/
  Module_constant* modRequiredTemp = new Module_constant();
  modRequiredTemp->m_config.constValue = 45;


/* */
  connect(    //CONNECT DC_INC-> OUT
    pidMod,
    pidMod->getSignal("DutyCyclePWMinc"),
    modRouterOut,
    pwmInc
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

  connect(    //CONNECT constant Signal -> requiredTemperature of PID
    modRequiredTemp,
    modRequiredTemp->getSignal("constSig"),
    pidMod,
    pidMod->getSlot("requiredTemperature")
  );









  initGlobalClock();

  return 0;
}

void triggerGlobalClock()
{
  if(recourcesAvailable){
    recourcesAvailable = false;
      globalClock.trigger();
    recourcesAvailable = true;
  }
}
void initGlobalClock()
{
  Clock clock(std::chrono::milliseconds(200), triggerGlobalClock );
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
