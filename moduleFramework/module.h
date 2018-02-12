#ifndef module__h
#define module__h


#define debugMode 0


#include <climits>
#include <vector>
#include <string>
#include <map>
#include <set>

#include "control.h"


struct mySqlSignal{
  std::string DeviceID;
  std::string PortType;
  std::string Port;
  std::string Pin;
  bool operator==(const mySqlSignal& otherSig) const
  {
    return (
      this->DeviceID  ==  otherSig.DeviceID   &&
      this->PortType  ==  otherSig.PortType   &&
      this->Port      ==  otherSig.Port       &&
      this->Pin       ==  otherSig.Pin
    );
  }
  bool operator < ( const mySqlSignal& otherSig ) const
  {
    return(
      ( this->DeviceID <  otherSig.DeviceID )  ||
      ( this->DeviceID == otherSig.DeviceID && this->PortType <  otherSig.PortType ) ||
      ( this->DeviceID == otherSig.DeviceID && this->PortType == otherSig.PortType && this->Port <  otherSig.Port ) ||
      ( this->DeviceID == otherSig.DeviceID && this->PortType == otherSig.PortType && this->Port == otherSig.Port && this->Pin <  otherSig.Pin )
    );
  }
};
/*struct mySqlSignalCompare
{
   bool operator() (const mySqlSignal& lhs, const mySqlSignal& rhs) const;
};
*/
struct Slot
{
  int* value;
  int min = INT_MIN;
  int max = INT_MAX;
  bool synced = false;
};

struct Signal
{
  int value;
  int min = INT_MIN;
  int max = INT_MAX;
  std::vector<Slot*> slots;
};
class Module
{
public:
  Signal* getSignal(std::string signalName);
  Slot* getSlot(std::string slotName);
  void addPostModule(Module* postModule);
  void trigger();
  ~Module();
private:

protected:
  std::vector<Module*> m_postModules;
  std::map<std::string, Signal*> m_signals;
  std::map<std::string, Slot*> m_slots;
  Signal* createSignal(std::string signalName);
  Slot* createSlot(std::string slotName);
  void triggerNext();
  virtual void process();
  void emitSignal(std::string signalName, int value);
  int getSignalValue(std::string slotName);

};

class ClockDistributer{
  public:
    void trigger();
    void addDestination(Module* destModule);
    void rmDestination(Module* destModule);
  protected:
    std::set<Module*> m_destModules;
};


struct mysql_config
{
  unsigned int ID;
};


struct module_config
{
  mysql_config config;
};

// ____Module which provides a constant Signal defined by Config________________
struct module_config_constant : public module_config
{
  int constValue = 0;
};

class Module_constant: public Module
{
public:
  Module_constant();
  ~Module_constant();
  module_config_constant m_config;
private:

protected:
  void process() override;
};
// _____________________________________________________________________________

// ____Module which prints an Value for debugging to console,____________________
//     identifier is given by config
struct module_config_debug : public module_config
{
  std::string identifier = "Debug";
};

class Module_debug : public Module
{
  public:
    Module_debug();
    module_config_debug m_config;
  private:

  protected:
    void process() override;
};


// _____________________________________________________________________________

//____Module which controls an 3-way-valve______________________________________
//    IN:  requiredTemperature , actualTemperatur
//    OUT: DutyCyclePWMinc, DutyCyclePWMdec,
struct module_config_3WayValve : public module_config
{
  config_ServoPWM* pwmConfig;
  config_PID* pidConfig;
};


class Module_3WayValve : public Module
{
public:
  Module_3WayValve();
  module_config_3WayValve m_config;
protected:
  PID pid;
  ServoPWM pwm;
  void process() override;
private:
};
//______________________________________________________________________________
//____Module which provides an 2-Point Controller_______________________________
//    IN:  T1, T2
//    OUT: state (1/0)
struct module_config_2Point : public module_config
{
  int dT_on = 8;
  int dT_off = 1;
};


class Module_2Point : public Module
{
public:
  Module_2Point();
  module_config_2Point m_config;
protected:
  int outState = 0;
  void process() override;
private:
};
//______________________________________________________________________________
//____Module which inverts an Bool Signal_______________________________________
//    IN:  S (1/0)
//    OUT: S (0/1)
class Module_Inverter : public Module
{
public:
  Module_Inverter();
  module_config m_config;
protected:
  void process() override;
private:
};
//______________________________________________________________________________

#endif
