#ifndef signalRouter__h
#define signalRouter__h
#include "module.h"
#include <map>
namespace SignalRouter
{
    class SignalRouterIn : public Module::Module
    {
    public:
      *Signal createSignalIfNotexist(const& mySqlSignal _extSignal);
    private:
      std::map<mySqlSignal, Signal> signalMap;
    protected:

    }

    class SignalRouterOut : public Module::Module
    {
    public:
      *Slot createSlotIfNotExist(const& mySqlSignal _extSignal);
    private:
      std::map<mySqlSignal, Slot> slotMap;
    protected:
    }
}
#endif
