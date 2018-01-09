#include "module.h"
#include <iostream>
extern ClockDistributer globalClock;

// ____Module___________________________________________________________________
void Module::emitSignal(std::string signalName, int value)
{
  Signal* signal = nullptr;
  try { signal = m_signals.at(signalName); }
  catch (const std::exception& e)
  {
    if(debug){
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
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
    for(auto&& slot: signal->slots){
      slot->synced=true;
    }
  }
}

int Module::getSignalValue(std::string slotName)
{
  Slot* slot = nullptr;
  int preRet = 0;
  try { slot = m_slots.at(slotName); }
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
        preRet = slot->min;
      }
    }
  }
  return preRet;
}

void Module::trigger()
{
  bool allInputsSynced=true;
  for(auto slotName_slot : m_slots){
    if(!slotName_slot.second->synced){
      allInputsSynced=false;
    }
  }
  if(allInputsSynced){
    for(auto slotName_slot : m_slots){
      slotName_slot.second->synced=false;
    }
    process();
    triggerNext();
  }
}

void Module::triggerNext()
{
  if(debug) std::cout << "Module::triggerNext" << std::endl;
  for(auto postModule : m_postModules){
    postModule->trigger();
  }
}

void Module::process()
{
  if(debug){
    std::cout << "virtual Module::process():" << std::endl;
    for(auto&& keyValPair : m_signals){
      std::cout << "    " << keyValPair.first << keyValPair.second->value << std::endl;
    }
  }
}

Signal* Module::createSignal(std::string signalName)
{
  if(m_signals.count(signalName) == 0){
    Signal* newSignal = new Signal();
    m_signals[signalName] = newSignal;
    return newSignal;
  }else{
    return m_signals.at(signalName);
  }
}

Slot* Module::createSlot(std::string slotName)
{
  if(m_slots.count(slotName) == 0){
    Slot* newSlot = new Slot();
    m_slots[slotName] = newSlot;
    return newSlot;
  }else{
    return m_slots.at(slotName);
  }
}

Signal* Module::getSignal(std::string signalName)
{
  try { return m_signals.at(signalName); }
  catch (const std::exception& e)
  {
    if(debug){
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return nullptr;
  }
}

Slot* Module::getSlot(std::string slotName)
{
  try { return m_slots.at(slotName); }
  catch (const std::exception& e)
  {
    if(debug){
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return nullptr;
  }
}

Module::~Module()
{
  if(debug){
    std::cout << "debug:~Module()" << std::endl;
  }
  for(auto&& keyValPair : m_signals){
    delete keyValPair.second;
    if(debug){
      std::cout << "deletet Signal: " << keyValPair.first << std::endl;
    }
  }
  for(auto&& keyValPair : m_slots){
    delete keyValPair.second;
    if(debug){
      std::cout << "deletet Slot: " << keyValPair.first << std::endl;
    }
  }
}
// ____ClockDistributer_________________________________________________________
void ClockDistributer::trigger()
{
  for(auto&& destModule : m_destModules){
    destModule->trigger();
  }
}
void ClockDistributer::addDestination(Module* destModule)
{
  m_destModules.insert(destModule);
}
void ClockDistributer::rmDestination(Module* destModule)
{
  if(m_destModules.erase(destModule) < 1){
    if(debug){
      std::cout << "ClockDistributer::rmDestination : Element to remove"
      << "is not in m_destModules" << std::endl;
    }
  }
}
// ____mySqlSignal______________________________________________________________
bool mySqlSignal::operator == (mySqlSignal const& otherSig)
{
  return (
    this->DeviceID  ==  otherSig.DeviceID   &&
    this->PortType  ==  otherSig.PortType   &&
    this->Port      ==  otherSig.Port       &&
    this->Pin       ==  otherSig.Pin
  );
}
// ____Module_constant__________________________________________________________
Module_constant::Module_constant()
{
  globalClock.addDestination(this);
  createSignal("constSig");
}

Module_constant::~Module_constant()
{
  globalClock.rmDestination(this);
}

void Module_constant::process()
{
  if(debug) std::cout << "Module_constant::process" << std::endl;
  emitSignal("constSig", m_config.constValue);
}
// ____Module_debug_____________________________________________________________
Module_debug::Module_debug()
{
  createSlot("debugSlot");
}

void Module_debug::process()
{
  if(debug) std::cout << "Module_debug::process" << std::endl;
  std::cout << "Module_debug::" << m_config.identifier << " = "
  << getSignalValue("debugSlot") << std::endl;
}

// _____________________________________________________________________________
