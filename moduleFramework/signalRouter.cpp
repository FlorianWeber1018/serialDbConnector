#include "signalRouter.h"
#include <map>
#include <iostream>
Signal* SignalRouterIn::createSignalIfNotexist(mySqlSignal const& extSignal)
{

  for(auto&& keyValPair : m_signalMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Signal* newSignal = new Signal();
  std::cout<< m_signalMap[newSignal] ;
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
  //Ym_slotMap[newSlot] = mySqlSignal;
  return newSlot;
}
