#include "mainModuleFramework.h"
#include "clock.h"
#include "control.h"
#include "paramRouter.h"
#include <iostream>
#include <thread>

ClockDistributer globalClock;
ParamRouter *globalParams =
  new ParamRouter("localhost", 3306, "IoD", "637013", "heizung");
volatile bool recourcesAvailable = true;

int main() {
  // std::this_thread::sleep_for(std::chrono::seconds(15));
  /*Module* _1x1 = new Module();
  _1x1->*/
  SignalRouterIn *modRouterIn =
      new SignalRouterIn("localhost", 3306, "IoD", "637013", "heizung");

  SignalRouterOut *modRouterOut =
      new SignalRouterOut("localhost", 3306, "IoD", "637013", "heizung");

  mySqlSignal actualTemp;
  actualTemp.DeviceID = "/dev/ttyACM0";
  actualTemp.PortType = "A";
  actualTemp.Port = "0";
  actualTemp.Pin = "0";

  mySqlSignal pwmInc;
  pwmInc.DeviceID = "/dev/ttyACM0";
  pwmInc.PortType = "I";
  pwmInc.Port = "2";
  pwmInc.Pin = "5";

  mySqlSignal pwmDec;
  pwmDec.DeviceID = "/dev/ttyACM0";
  pwmDec.PortType = "I";
  pwmDec.Port = "2";
  pwmDec.Pin = "6";

  Module_3WayValve *pidMod = new Module_3WayValve();

  Module_constant *modRequiredTemp = new Module_constant();
  modRequiredTemp->m_config.constValue = 45;

  mySqlSignal WWTemp;
  WWTemp.DeviceID = "/dev/ttyACM0";
  WWTemp.PortType = "A";
  WWTemp.Port = "0";
  WWTemp.Pin = "1";

  mySqlSignal HKP0;
  HKP0.DeviceID = "/dev/ttyACM0";
  HKP0.PortType = "I";
  HKP0.Port = "2";
  HKP0.Pin = "7";

  Module_2Point *WW = new Module_2Point();

  Module_constant *modWWTemp = new Module_constant();
  modWWTemp->m_config.constValue = 45;

  Module_Inverter *modInv = new Module_Inverter();

  Module_constant *modTRUE = new Module_constant();
  modTRUE->m_config.constValue = 1;

  Module_constant *modFALSE = new Module_constant();
  modFALSE->m_config.constValue = 0;
  /* */
  connect( // CONNECT DC_INC-> OUT
      pidMod, pidMod->getSignal("DutyCyclePWMinc"), modRouterOut, pwmInc);

  connect( // CONNECT DC_DEC-> OUT
      pidMod, pidMod->getSignal("DutyCyclePWMdec"), modRouterOut, pwmDec);

  connect( // CONNECT Temperature Signal -> actualTemperatur of PID
      modRouterIn, actualTemp, pidMod, pidMod->getSlot("actualTemperature"));

  connect( // CONNECT constant Signal -> requiredTemperature of PID
      modRequiredTemp, modRequiredTemp->getSignal("constSig"), pidMod,
      pidMod->getSlot("requiredTemperature"));

  connect( // CONNECT constant Signal -> !EN of PID
      modInv, modInv->getSignal("S"), pidMod, pidMod->getSlot("!EN"));

  connect( // CONNECT Inverted Signal -> !EN of PID
      WW, WW->getSignal("outState"), modInv, modInv->getSlot("S"));

  connect( // connect WWTemp min -> T2
      modWWTemp, modWWTemp->getSignal("constSig"), WW, WW->getSlot("T2"));

  connect( // connect WWTemp min -> T2
      modRouterIn, WWTemp, WW, WW->getSlot("T1"));

  connect( // connect outState -> pumpe heizkreis
      WW, WW->getSignal("outState"), modRouterOut, HKP0);

  /*
    Module_debug* modMin = new Module_debug();
    modMin->m_config.identifier = "minute";

    Module_debug* modSec = new Module_debug();
    modSec->m_config.identifier = "second";

    connectToTime(    //CONNECT minute Signal -> debug
      modRouterIn,
      "minute",
      modMin,
      modMin->getSlot("debugSlot")
    );

    connectToTime(    //CONNECT second Signal -> debug
      modRouterIn,
      "second",
      modSec,
      modSec->getSlot("debugSlot")
    );
  */

  initGlobalClock();

  return 0;
}

void triggerGlobalClock() {
  if (recourcesAvailable) {
    recourcesAvailable = false;
    globalClock.trigger();
    recourcesAvailable = true;
  }
}
void initGlobalClock() {
  Clock clock(std::chrono::milliseconds(200), triggerGlobalClock);
  if (debugMode)
    std::cout << "Now starting mainloop" << std::endl;
  clock.run();
  while (1) {
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
