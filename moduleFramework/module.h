#ifndef module__h
#define module__h

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
  int executionLevel;
  std::vector<Slot*> slots;
};
class Module
{
public:
  std::vector<Signal*> m_signals;
  std::vector<Slot*> m_slots;
  std::vector<Module*> m_postModules;
  void trigger();

private:

protected:
  void triggerNext();
  virtual void process();
  void emitSignal(int signalNumber, int value);
  int getSignal(int slotNumber);
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
