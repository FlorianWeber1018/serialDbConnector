#ifndef module__h
#define module__h


#define debugMode 2


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
};

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
  unsigned int ID = 0;
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

// ____Module which provides a constant Signal defined by Config________________
class Module_constant: public Module
{
public:
  Module_constant();
  Module_constant(unsigned int ID);
  ~Module_constant();

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
    Module_debug(unsigned int ID);
  private:

  protected:
    void process() override;
};


// _____________________________________________________________________________

//____Module which controls an 3-way-valve______________________________________
//    IN:  requiredTemperature , actualTemperatur
//    OUT: DutyCyclePWMinc, DutyCyclePWMdec,



class Module_3WayValve : public Module
{
public:
  Module_3WayValve();
  Module_3WayValve(unsigned int ID);
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

class Module_2Point : public Module
{
public:
  Module_2Point();
  Module_2Point(unsigned int ID);

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
  Module_Inverter(unsigned int ID);
protected:
  void process() override;
private:
};
//______________________________________________________________________________
//____Module which controlls an wood fired oven_________________________________
//    IN:  S, VL_Wood, RL_Wood
//    OUT: Fan (0/1)

class Module_woodstove : public Module
{

};
//______________________________________________________________________________
#endif
