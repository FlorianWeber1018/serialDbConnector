#ifndef module__h
#define module__h
#define debug = 1



#include <vector>
#include <string>

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
  int min;
  int max;
  bool synced = false;
};

struct Signal
{
  int value;
  int min;
  int max;
  std::vector<Slot*> slots;
};
class Module
{
public:
  Signal* getSignal();
  Slot* getSlot();
  getSlot()
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
  int getSignalvalue(std::string slotName);

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
struct module_config_constant : private module_config
{
  int const_value;
};

class module_constant: public Module
{
public:
  module_constant();
private:

protected:

};
// _____________________________________________________________________________


#endif
