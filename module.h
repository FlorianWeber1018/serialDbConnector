#ifndef module__h
#define module__h

#include <vector>
#include <string>
namespace SignalSlots{

  struct mySqlSignal{
    const& std::string DeviceID;
    const& std::string PortType;
    const& std::string Port;
    const& std::string Pin;
  }

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
    vector<Slot*> slots;
  };

  class Module
  {
  public:
    vector<Signal> m_signals;
    vector<Slot> m_slots;
    vector<Module*> m_postModules;
    void trigger();

  private:

  protected:

    void triggerNext();
    virtual void process();
    void emitSignal(int signalNumber, int value);
    int getSignal(int slotNumber);
  };

  void connect(SignalRouterIn* signalRouter, const& mySqlSignal _extSignal, Module* receiver, Slot* _Slot);       //done
  void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot);                                                   //done
  void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, const& mySqlSignal _extSignal);  //done
}




#endif
