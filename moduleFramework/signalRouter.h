#ifndef signalRouter__h
#define signalRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <map>



class SignalRouterIn : public Module
{
public:

  Signal* createSignalIfNotexist(mySqlSignal const& extSignal);
  mysqlcon* mySqlConnection;
private:
  std::map<Signal*, mySqlSignal> m_signalMap;
protected:
  void process() override;
};

class SignalRouterOut : public Module, public mysqlcon
{
public:
  Slot* createSlotIfNotExist(mySqlSignal const& extSignal);
private:
  std::map<Slot*, mySqlSignal> m_slotMap;
protected:
  void process() override;
};

#endif
