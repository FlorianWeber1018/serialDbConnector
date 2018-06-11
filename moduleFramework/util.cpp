#include "util.h"
#include <chrono>
#include <thread>

void connectToTime(SignalRouterIn* signalRouter, const std::string& key, Module* receiver, Slot* _Slot)
{
  connect( signalRouter, signalRouter->getTimeSignal(key), receiver, _Slot );
}
void connect(SignalRouterIn* signalRouter, mySqlSignal const& _extSignal, Module* receiver, Slot* _Slot)
{
  connect( signalRouter, signalRouter->createSignalIfNotexist(_extSignal) , receiver, _Slot );
}

void connectToSoftwareButton(SignalRouterIn* signalRouter, unsigned int key, Module* receiver, Slot* _Slot)
{
    connect( signalRouter, signalRouter->createSignalIfNotexist(key) , receiver, _Slot );
}

void connectToSoftwareMonitor(
  Module* sender, Signal* _Signal,
  SignalRouterOut* signalRouterOut, unsigned int key)
{
  connect( sender, _Signal ,
    signalRouterOut, signalRouterOut->createSlotIfNotexist(key));
}
void connectToSoftwareMonitor(
  SignalRouterIn* signalRouterIn, mySqlSignal const& _extSignal,
  SignalRouterOut* signalRouterOut, unsigned int key)
{
  connect( signalRouterIn, signalRouterIn->createSignalIfNotexist(_extSignal) ,
    signalRouterOut, signalRouterOut->createSlotIfNotexist(key));

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
