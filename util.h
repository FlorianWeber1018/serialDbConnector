#ifndef util__h
#define util__h
#include "signalRouter.h"
#include "module.h"

struct mySqlSignal{
  std::string DeviceID;
  std::string PortType;
  std::string Port;
  std::string Pin;
};

void connect(SignalRouterIn* signalRouter, const& mySqlSignal _extSignal, Module* receiver, Slot* _Slot);       //done
void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot);                                                   //done
void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, const& mySqlSignal _extSignal);  //done

#endif
