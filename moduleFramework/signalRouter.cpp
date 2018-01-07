#include "signalRouter.h"
Signal* SignalRouterIn::createSignalIfNotexist(mySqlSignal const& extSignal)
{

  return &signalMap[extSignal];
}

Slot* SignalRouterOut::createSlotIfNotExist(mySqlSignal const& extSignal)
{
  return &slotMap[extSignal];
}
