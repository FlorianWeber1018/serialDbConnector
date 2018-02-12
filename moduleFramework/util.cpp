#include "util.h"
#include <chrono>
#include <thread>

void connectToServerT(SignalRouterIn* signalRouter, Time const& time, Module* receiver, Slot* _Slot)
{
  connect( signalRouter, signalRouter->getTimeSignal(time) , receiver, _Slot );
}

void connect(SignalRouterIn* signalRouter, mySqlSignal const& _extSignal, Module* receiver, Slot* _Slot)
{
  connect( signalRouter, signalRouter->createSignalIfNotexist(_extSignal) , receiver, _Slot );
}

void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot)
{
  _Slot->value = &(_Signal->value);
  _Signal->slots.push_back(_Slot);
  sender->addPostModule(receiver);
}

void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, mySqlSignal const& _extSignal)
{
  connect( sender, _Signal,  signalRouter, signalRouter->createSlotIfNotexist(_extSignal));
}

/*
void connect(Module* sender, Signal* _Signal, SignalRouter* signalRouter, mySqlSignal const& _extSignal)
{
  //connect( sender, _Signal, signalRouter, signalRouter->createSlotIfNotExist(_extSignal) );
  MACH NEU!!!
}
*/
