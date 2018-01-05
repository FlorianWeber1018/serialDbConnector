#include "util.h"

void connect(SignalRouterIn* signalRouter, mySqlSignal const& _extSignal, Module* receiver, Slot* _Slot)
{
  connect( signalRouter->createSignalIfNotexist(_extSignal) , receiver, _Slot );
}

void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot)
{
  _Slot->value = &(_Signal->value);
  _Signal->slots.append(_Slot);
  sender->m_postModules.append(receiver);
}

void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, mySqlSignal const& _extSignal)
{
  connect( sender, _Signal, signalRouter, signalRouter->createSlotIfNotExist(_extSignal) );
}
