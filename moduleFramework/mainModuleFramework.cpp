#include "mainModuleFramework.h"
#include "clock.h"
#include "control.h"
#include "paramRouter.h"
#include <iostream>
#include <thread>

ClockDistributer globalClock;
ParamRouter* globalParams = nullptr;
SignalRouterIn* modRouterIn = nullptr;
SignalRouterOut* modRouterOut = nullptr;
ModConMan* modConMan = nullptr;

volatile bool recourcesAvailable = true;

int main() {
  // std::this_thread::sleep_for(std::chrono::seconds(15));
  /*Module* _1x1 = new Module();
  _1x1->*/
  globalParams = new ParamRouter("localhost", 3306, "IoD", "637013", "heizung");
  modRouterIn = new SignalRouterIn("localhost", 3306, "IoD", "637013", "heizung");
  modRouterOut = new SignalRouterOut("localhost", 3306, "IoD", "637013", "heizung");
  modConMan = new modConMan("localhost", 3306, "IoD", "637013", "heizung");


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
