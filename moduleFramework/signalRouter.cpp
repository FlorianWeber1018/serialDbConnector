#include "signalRouter.h"
#include <map>
Signal* SignalRouterIn::createSignalIfNotexist(mySqlSignal const& extSignal)
{

  for(auto&& keyValPair : m_signalMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Signal* newSignal = new Signal();
  m_signalMap[newSignal] = mySqlSignal;
  return newSignal;
}

Slot* SignalRouterOut::createSlotIfNotExist(mySqlSignal const& extSignal)
{
  for(auto&& keyValPair : m_slotMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Slot* newSlot = new Slot();
  m_slotMap[newSlot] = mySqlSignal;
  return newSlot;
}
