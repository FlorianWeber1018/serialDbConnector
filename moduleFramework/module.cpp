#include "module.h"
#include <iostream>
extern ClockDistributer globalClock;
extern ParamRouter globalParams;
unsigned int

    // ____Module___________________________________________________________________
    void
    Module::emitSignal(std::string signalName, int value) {
  Signal *signal = nullptr;
  try {
    signal = m_signals.at(signalName);
  } catch (const std::exception &e) {
    if (debugMode) {
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return;
  }
  if (signal != nullptr) {
    if (value > signal->max) {
      signal->value = signal->max;
    } else {
      if (value < signal->min) {
        signal->value = signal->min;
      } else {
        signal->value = value;
      }
    }
    for (auto &&slot : signal->slots) {
      slot->synced = true;
    }
  }
}

int Module::getSignalValue(std::string slotName) {
  Slot *slot = nullptr;
  int preRet = 0;
  try {
    slot = m_slots.at(slotName);
  } catch (const std::exception &e) {
    std::cout << "exception was cought : " << e.what() << std::endl;
    return preRet;
  }
  if (slot->value == nullptr) {
    return preRet;
  }

  if (slot != nullptr) {
    preRet = *(slot->value);
    if (preRet > slot->max) {
      preRet = slot->max;
    } else {
      if (preRet < slot->min) {
        preRet = slot->min;
      }
    }
  }
  return preRet;
}

void Module::trigger() {
  bool allInputsSynced = true;
  for (auto slotName_slot : m_slots) {
    if ((!slotName_slot.second->synced) &&
        slotName_slot.second->value != nullptr) {
      allInputsSynced = false;
    }
  }
  if (allInputsSynced) {
    for (auto slotName_slot : m_slots) {
      slotName_slot.second->synced = false;
    }
    process();
    triggerNext();
  }
}

void Module::triggerNext() {
  if (debugMode)
    std::cout << "Module::triggerNext" << std::endl;
  for (auto postModule : m_postModules) {
    if (debugMode)
      std::cout << "1 Module triggered" << std::endl;
    postModule->trigger();
  }
}

void Module::process() {
  if (debugMode) {
    std::cout << "virtual Module::process():" << std::endl;
    for (auto &&keyValPair : m_signals) {
      std::cout << "    " << keyValPair.first << keyValPair.second->value
                << std::endl;
    }
  }
}

Signal *Module::createSignal(std::string signalName) {
  if (m_signals.count(signalName) == 0) {
    Signal *newSignal = new Signal();
    m_signals[signalName] = newSignal;
    return newSignal;
  } else {
    return m_signals.at(signalName);
  }
}

Slot *Module::createSlot(std::string slotName) {
  if (m_slots.count(slotName) == 0) {
    Slot *newSlot = new Slot();
    m_slots[slotName] = newSlot;
    newSlot->value = nullptr;
    return newSlot;
  } else {
    return m_slots.at(slotName);
  }
}

Signal *Module::getSignal(std::string signalName) {
  try {
    return m_signals.at(signalName);
  } catch (const std::exception &e) {
    if (debugMode) {
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return nullptr;
  }
}
void Module::addPostModule(Module *postModule) {
  m_postModules.push_back(postModule);
}

Slot *Module::getSlot(std::string slotName) {
  try {
    return m_slots.at(slotName);
  } catch (const std::exception &e) {
    if (debugMode) {
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return nullptr;
  }
}

Module::~Module() {
  if (debugMode) {
    std::cout << "debug:~Module()" << std::endl;
  }
  for (auto &&keyValPair : m_signals) {
    delete keyValPair.second;
    if (debugMode) {
      std::cout << "deletet Signal: " << keyValPair.first << std::endl;
    }
  }
  for (auto &&keyValPair : m_slots) {
    delete keyValPair.second;
    if (debugMode) {
      std::cout << "deletet Slot: " << keyValPair.first << std::endl;
    }
  }
}
// ____ClockDistributer_________________________________________________________
void ClockDistributer::trigger() {
  for (auto &&destModule : m_destModules) {
    destModule->trigger();
  }
}
void ClockDistributer::addDestination(Module *destModule) {
  m_destModules.insert(destModule);
}
void ClockDistributer::rmDestination(Module *destModule) {
  if (m_destModules.erase(destModule) < 1) {
    if (debugMode) {
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
    ( lhs.DeviceID == rhs.DeviceID && lhs.PortType == rhs.PortType && lhs.Port <
rhs.Port ) || ( lhs.DeviceID == rhs.DeviceID && lhs.PortType == rhs.PortType &&
lhs.Port == rhs.Port && lhs.Pin <  rhs.Pin )
  );
}*/
// ____Module_constant__________________________________________________________
Module_constant::Module_constant() {
  globalClock.addDestination(this);
  createSignal("constSig");

  mySqlParam tempParamKey; // create Key to Config Param
  if (this->ID == 0) {
    this.ID = globalParams.getNextAvID();
  }
  tempParamKey.ID = this->ID;
  tempParamKey.paramKey = "constSig";
  globalParams.createParamIfNotExist(tempParamKey, 0); // Create Line on Server
}
Module_constant::Module_constant(unsigned int ID);
{ this->ID = ID; }
Module_constant::~Module_constant() { globalClock.rmDestination(this); }

void Module_constant::process() {
  tempParamKey.ID = this->ID;
  tempParamKey.paramKey = "constSig";
  emitSignal("constSig", globalParams.getParam(tempParamKey));
}
// ____Module_debug_____________________________________________________________
Module_debug::Module_debug() {
  createSlot("debugSlot");
  mySqlParam tempParamKey; // create Key to Config Param
  if (this->ID == 0) {
    this.ID = globalParams.getNextAvID();
  }
  tempParamKey.ID = this->ID;
  tempParamKey.paramKey = "identifier";
  globalParams.createParamIfNotExist(tempParamKey, 0); // Create Line on Server
}
Module_debug::Module_debug(unsigned int ID) { this->ID = ID; }
void Module_debug::process() {
  tempParamKey.ID = this->ID;
  tempParamKey.paramKey = "identifier";
  std::cout << "Module_debug::" << globalParams.getParam(tempParamKey) << " = "
            << getSignalValue("debugSlot") << std::endl;
}
// ____Module_3WayValve_________________________________________________________
Module_3WayValve::Module_3WayValve() {
  createSlot("requiredTemperature");
  createSlot("actualTemperature");
  createSlot("!EN");
  createSignal("DutyCyclePWMinc");
  createSignal("DutyCyclePWMdec");
  mySqlParam tempParamKey; // create Key to Config Param
  if (this->ID == 0) {
    this.ID = globalParams->getNextAvID;
  }
  tempParamKey.ID = this->ID;

  tempParamKey.paramKey = "kp";
  globalParams.createParamIfNotExist(tempParamKey, 3000);
  tempParamKey.paramKey = "up_max";
  globalParams.createParamIfNotExist(tempParamKey, INT_MAX);
  tempParamKey.paramKey = "up_min";
  globalParams.createParamIfNotExist(tempParamKey, INT_MIN);

  tempParamKey.paramKey = "ki";
  globalParams.createParamIfNotExist(tempParamKey, 0);
  tempParamKey.paramKey = "ui_max";
  globalParams.createParamIfNotExist(tempParamKey, INT_MAX);
  tempParamKey.paramKey = "ui_min";
  globalParams.createParamIfNotExist(tempParamKey, INT_MIN);

  tempParamKey.paramKey = "kd";
  globalParams.createParamIfNotExist(tempParamKey, 0);
  tempParamKey.paramKey = "ud_max";
  globalParams.createParamIfNotExist(tempParamKey, INT_MAX);
  tempParamKey.paramKey = "ud_min";
  globalParams.createParamIfNotExist(tempParamKey, INT_MIN);

  tempParamKey.paramKey = "input_max";
  globalParams.createParamIfNotExist(tempParamKey, 30000);
  tempParamKey.paramKey = "input_min";
  globalParams.createParamIfNotExist(tempParamKey, -30000);

  tempParamKey.paramKey = "incPWM_max";
  globalParams.createParamIfNotExist(tempParamKey, 150);
  tempParamKey.paramKey = "incPWM_min";
  globalParams.createParamIfNotExist(tempParamKey, 30);

  tempParamKey.paramKey = "decPWM_max";
  globalParams.createParamIfNotExist(tempParamKey, 150);
  tempParamKey.paramKey = "decPWM_min";
  globalParams.createParamIfNotExist(tempParamKey, 30);
}
Module_3WayValve::Module_3WayValve(unsigned int ID) { this->ID = ID; }
void Module_3WayValve::process() {
  int DC_inc, DC_dec;
  if (getSignalValue("!EN")) {
    tempParamKey.ID = this->ID;
    tempParamKey.paramKey = "decPWM_max";
    DC_dec = globalParams.getParam(tempParamKey),

    DC_inc = 0;
  } else {
    pid.syncParam(this->ID);
    pwm.syncParam(this->ID);
    int y = static_cast<int>(pid.getOutput(
        static_cast<float>(getSignalValue("actualTemperature")),
        static_cast<float>(getSignalValue("requiredTemperature"))));
    pwm.getOutput(DC_inc, DC_dec, y);
  }

  emitSignal("DutyCyclePWMinc", DC_inc);
  emitSignal("DutyCyclePWMdec", DC_dec);
}
// ____Module_2Point____________________________________________________________
Module_2Point::Module_2Point() {
  createSlot("T1");
  createSlot("T2");
  createSignal("outState");
  // std::cout << "constructor 2point" <<  std::endl;
}
void Module_2Point::process() {
  int diff = getSignalValue("T1") - getSignalValue("T2");
  if (debugMode)
    std::cout << "diff: " << diff << std::endl;
  if (diff >= m_config.dT_on) {
    outState = 1;
  } else if (diff <= m_config.dT_off) {
    outState = 0;
  }
  emitSignal("outState", outState);
}
// ____MODULE_Inverter__________________________________________________________
Module_Inverter::Module_Inverter() {
  createSlot("S");
  createSignal("S");
}
void Module_Inverter::process() {
  int value = getSignalValue("S");
  if (value <= 1 && value >= 0) {
    if (value == 1) {
      value = 0;
    } else {
      value = 1;
    }
  }
  emitSignal("S", value);
}
// _____________________________________________________________________________
