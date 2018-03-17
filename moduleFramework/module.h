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
struct mySqlParam{
  unsigned int ID;
  std::string paramKey;
  bool operator == (const mySqlParam& otherParam) const
  {
    return(
      this->ID        ==    otherParam.ID   &&
      this->paramKey  ==    otherParam.paramKey
    );
  }
  bool operator < (const mySqlParam& otherParam) const
  {
    return(
      (this->ID <   otherParam.ID)    ||
      (this->ID ==  otherParam.ID   &&  this->paramKey  <  otherParam.paramKey)
    );
  }
}
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



class Module_config
{

public:
  Module_config();
  Module_config(unsigned int ID);
  int get_param(std::string key);
  void reload_param();
  void reload_param(std::string key);
protected:
  void set_param(std::string key, int value);
  unsigned int ID;
  std::map<std::string, int> cnf;
};

// ____Module which provides a constant Signal defined by Config________________
class Module_constant: public Module
{
public:
  Module_constant();
  ~Module_constant();
  Module_config m_config;
private:

protected:
  void process() override;
};
// _____________________________________________________________________________

// ____Module which prints an Value for debugging to console,____________________
//     identifier is given by config


class Module_debug : public Module
{
  public:
    Module_debug();
    Module_config m_config;
  private:

  protected:
    void process() override;
};


// _____________________________________________________________________________

//____Module which controls an 3-way-valve______________________________________
//    IN:  requiredTemperature , actualTemperatur
//    OUT: DutyCyclePWMinc, DutyCyclePWMdec,
struct Module_config_3WayValve : public Module_config
{
  config_ServoPWM* pwmConfig;
  config_PID* pidConfig;
};


class Module_3WayValve : public Module
{
public:
  Module_3WayValve();
  Module_config m_config;
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
struct Module_config_2Point : public Module_config
{
  int dT_on = 8;
  int dT_off = 1;
};


class Module_2Point : public Module
{
public:
  Module_2Point();
  Module_config_2Point m_config;
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
  Module_config m_config;
protected:
  void process() override;
private:
};
//______________________________________________________________________________
//____Module which controlls an wood fired oven_________________________________
//    IN:  S, VL_Wood, RL_Wood
//    OUT: Fan (0/1)
struct Module_config_woodstove : public Module_config
{
  int dT_on = 8;
  int dT_off = 2;
};
class Module_woodstove : public Module
{

};
//______________________________________________________________________________
#endif
