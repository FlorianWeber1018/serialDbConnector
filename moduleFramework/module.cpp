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
    if(debugMode){
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
    if((!slotName_slot.second->synced) && slotName_slot.second.value != nullptr){
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
  if(debugMode) std::cout << "Module::triggerNext" << std::endl;
  for(auto postModule : m_postModules){
    if (debugMode) std::cout << "1 Module triggered" << std::endl;
    postModule->trigger();
  }
}

void Module::process()
{
  if(debugMode){
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
    newSlot->value = nullptr;
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
    if(debugMode){
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return nullptr;
  }
}
void Module::addPostModule(Module* postModule)
{
  m_postModules.push_back(postModule);
}

Slot* Module::getSlot(std::string slotName)
{
  try { return m_slots.at(slotName); }
  catch (const std::exception& e)
  {
    if(debugMode){
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return nullptr;
  }
}

Module::~Module()
{
  if(debugMode){
    std::cout << "debug:~Module()" << std::endl;
  }
  for(auto&& keyValPair : m_signals){
    delete keyValPair.second;
    if(debugMode){
      std::cout << "deletet Signal: " << keyValPair.first << std::endl;
    }
  }
  for(auto&& keyValPair : m_slots){
    delete keyValPair.second;
    if(debugMode){
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
    if(debugMode){
      std::cout << "ClockDistributer::rmDestination : Element to remove"
      << "is not in m_destModules" << std::endl;
    }
  }
}
// ____mySqlSignal______________________________________________________________



/*bool mySqlSignalCompare::operator() (
  const mySqlSignal& lhs, const mySqlSignal& rhs) const
{
  return(
    ( lhs.DeviceID <  rhs.DeviceID )  ||
    ( lhs.DeviceID == rhs.DeviceID && lhs.PortType <  rhs.PortType ) ||
    ( lhs.DeviceID == rhs.DeviceID && lhs.PortType == rhs.PortType && lhs.Port <  rhs.Port ) ||
    ( lhs.DeviceID == rhs.DeviceID && lhs.PortType == rhs.PortType && lhs.Port == rhs.Port && lhs.Pin <  rhs.Pin )
  );
}*/
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
  //if(debugMode) std::cout << "Module_constant::process" << std::endl;
  emitSignal("constSig", m_config.constValue);
}
// ____Module_debug_____________________________________________________________
Module_debug::Module_debug()
{
  createSlot("debugSlot");
}

void Module_debug::process()
{
  //if(debugMode) std::cout << "Module_debug::process" << std::endl;
  std::cout << "Module_debug::" << m_config.identifier << " = "
  << getSignalValue("debugSlot") << std::endl;
}
// ____Module_3WayValve_________________________________________________________
Module_3WayValve::Module_3WayValve()
{
  createSlot("requiredTemperature");
  createSlot("actualTemperature");
  createSlot("!EN");
  createSignal("DutyCyclePWMinc");
  createSignal("DutyCyclePWMdec");
  m_config.pwmConfig = &(pwm.config);
  m_config.pidConfig = &(pid.config);

}
void Module_3WayValve::process()
{
  int DC_inc, DC_dec;
  if(getSignalValue("!EN")){
    DC_dec = m_config->pwmConfig.decPWM_max;
    DC_inc = 0;
  }else{
    int y = static_cast<int> (
      pid.getOutput(
        static_cast<float>( getSignalValue("actualTemperature") ),
        static_cast<float>( getSignalValue("requiredTemperature") )
      )
    );
    pwm.getOutput(DC_inc, DC_dec, y);
  }

  emitSignal("DutyCyclePWMinc", DC_inc);
  emitSignal("DutyCyclePWMdec", DC_dec);
}
// ____Module_2Point____________________________________________________________
Module_2Point::Module_2Point()
{
  createSlot("T1");
  createSlot("T2");
  createSignal("outState");
  //std::cout << "constructor 2point" <<  std::endl;
}
void Module_2Point::process()
{
  int diff = getSignalValue("T1")-getSignalValue("T2");
  if (debugMode) std::cout << "diff: " << diff << std::endl;
  if(diff >= m_config.dT_on){
    outState=1;
  }else if(diff <= m_config.dT_off){
    outState=0;
  }
  emitSignal("outState", outState);
}
// _____________________________________________________________________________
