#ifndef signalRouter__h
#define signalRouter__h

#include "module.h"
#include <map>
struct mySqlSignal;
    class SignalRouterIn : public Module
    {
    public:
      Signal* createSignalIfNotexist(mySqlSignal _extSignal);
    private:
      std::map<mySqlSignal, Signal> signalMap;
    protected:

    };

    class SignalRouterOut : public Module
    {
    public:
      Slot* createSlotIfNotExist(mySqlSignal _extSignal);
    private:
      std::map<mySqlSignal, Slot> slotMap;
    protected:
    };

#endif
