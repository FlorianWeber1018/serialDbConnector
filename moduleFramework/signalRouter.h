#ifndef signalRouter__h
#define signalRouter__h

#include "module.h"
#include <map>

class SignalRouterIn : public Module
{
public:
  Signal* createSignalIfNotexist(mySqlSignal const& extSignal);
private:
  std::map<mySqlSignal, Signal> signalMap;
protected:
};

class SignalRouterOut : public Module
{
public:
  Slot* createSlotIfNotExist(mySqlSignal const& extSignal);
private:
  std::map<mySqlSignal, Slot> slotMap;
protected:
};

#endif
