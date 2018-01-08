#include "module.h"
#include <iostream>
void Module::emitSignal(std::string signalName, int value)
{
  Signal* signal = nullptr;
  try { signal = m_signals.at(signalName); }
  catch (const std::exception& e)
  {
    std::cout << "exception was cought : " << e.what() << std::endl;
    return;
  }
  if(signal != nullptr){
    if(value > signal->max){
      signal->value = signal->max;
    }else{
      if(value < signal->min){
        signal->value = signal->min;
      }else{
        signal->value = value;
      }
    }
  }
}

int Module::getSignal(std::string slotName)
{
  Slot* slot = nullptr;
  int preRet = 0;
  try { slot = m_slots.at(slotName) }
  catch (const std::exception& e)
  {
    std::cout << "exception was cought : " << e.what() << std::endl;
    return preRet;
  }
    if(slot != nullptr){
    preRet = *(slot->value);
    if(preRet > slot->max){
      preRet = slot->max;
    }else{
      if(preRet < slot->min){
        preRet= slot->min;
      }
    }
  }
  return preRet;
}

void Module::trigger()
{
  bool allInputsSynced=true;
  for(auto slot : m_slots){
    if(!slot->synced){
      allInputsSynced=false;
    }
  }
  if(allInputsSynced){
    process();
    triggerNext();
  }
}

void Module::triggerNext()
{
  for(auto postModule : m_postModules){
    postModule->trigger();
  }
}

void Module::process()
{
  std::cout << "virtual method: Module::process() that does nothing" << std::endl;
}
bool mySqlSignal::operator == (mySqlSignal const& otherSig)
{
  return (
    this->DeviceID  ==  otherSig.DeviceID   &&
    this->PortType  ==  otherSig.PortType   &&
    this->Port      ==  otherSig.Port       &&
    this->Pin       ==  otherSig.Pin
  );
}

module_constant::module_constant()
{

}
