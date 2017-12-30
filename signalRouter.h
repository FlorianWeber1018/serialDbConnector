#ifndef signalRouter__h
#define signalRouter__h
#include "module.h"
namespace SignalRouter
{
    class SignalRouterIn : public Module
    {
    public:
      *Signal createSignalIfNotexist(const& mySqlSignal _extSignal);
    private:
      std::map<mySqlSignal, Signal> signalMap;
    protected:

    }
    class SignalRouterOut : public Module
    {
    public:
      *Slot createSlotIfNotExist(const& mySqlSignal _extSignal);
    private:
      std::map<mySqlSignal, Slot> signalMap;
    protected:
    }
}
#endif
