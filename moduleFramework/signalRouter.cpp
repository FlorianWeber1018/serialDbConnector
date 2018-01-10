#include "signalRouter.h"
Signal* SignalRouterIn::createSignalIfNotexist(mySqlSignal const& extSignal)
{

  for(auto&& keyValPair : m_signalMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Signal* newSignal = new Signal();
  m_signalMap[newSignal] = extSignal;
  return newSignal;
}
void SignalRouterIn::process()
{

}
// _____________________________________________________________________________
Slot* SignalRouterOut::createSlotIfNotExist(mySqlSignal const& extSignal)
{
  for(auto&& keyValPair : m_slotMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Slot* newSlot = new Slot();
  m_slotMap[newSlot] = extSignal;
  return newSlot;
}
void SignalRouterOut::process()
{

}
