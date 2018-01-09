#ifndef module__h
#define module__h
#define debug 0


#include <climits>
#include <vector>
#include <string>
#include <map>
#include <set>



struct mySqlSignal{
  std::string DeviceID;
  std::string PortType;
  std::string Port;
  std::string Pin;
  bool operator==(mySqlSignal const& otherSig);
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

  std::vector<Module*> m_postModules;
  void trigger();
  ~Module();
private:

protected:
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
    addDestination(Module* destModule);
    rmDestination(Module* destModule);
  protected:
    set<Module*> m_destModules;
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
#endif
