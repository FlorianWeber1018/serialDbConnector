#ifndef signalRouter__h
#define signalRouter__h

#include "module.h"
#include <map>

class SignalRouterIn : public Module
{
public:
  Signal* createSignalIfNotexist(mySqlSignal const& extSignal);
private:
  std::map<Signal*, mySqlSignal> m_signalMap;
protected:
};

class SignalRouterOut : public Module
{
public:
  Slot* createSlotIfNotExist(mySqlSignal const& extSignal);
private:
  std::map<Slot*, mySqlSignal> m_slotMap;
protected:
};

#endif
