#include "util.h"

void connect(SignalRouter* signalRouter, mySqlSignal const& _extSignal, Module* receiver, Slot* _Slot)
{
  connect( signalRouter, signalRouter->createSignalIfNotexist(_extSignal) , receiver, _Slot );
}

void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot)
{
  _Slot->value = &(_Signal->value);
  _Signal->slots.push_back(_Slot);
  sender->m_postModules.push_back(receiver);
}

void connect(Module* sender, Signal* _Signal, SignalRouter* signalRouter, mySqlSignal const& _extSignal)
{
  connect( sender, _Signal, signalRouter, signalRouter->createSlotIfNotExist(_extSignal) );
}
