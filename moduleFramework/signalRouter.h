#ifndef signalRouter__h
#define signalRouter__h

#include "module.h"
#include <map>
#include ".. /IoD/mysqlcon.h"

class SignalRouterIn : public Module//, public mysqlcon
{
public:
  Signal* createSignalIfNotexist(mySqlSignal const& extSignal);
private:
  std::map<Signal*, mySqlSignal> m_signalMap;
protected:
  void process() override;
};

class SignalRouterOut : public Module//, public mysqlcon
{
public:
  Slot* createSlotIfNotExist(mySqlSignal const& extSignal);
private:
  std::map<Slot*, mySqlSignal> m_slotMap;
protected:
  void process() override;
};

#endif
