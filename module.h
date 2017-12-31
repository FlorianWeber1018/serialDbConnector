#ifndef module__h
#define module__h

#include <vector>
#include <string>
namespace SignalSlots{

  struct mySqlSignal{
    std::string DeviceID;
    std::string PortType;
    std::string Port;
    std::string Pin;
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
    vector<Slot*> slots;
  };

  class Module
  {
  public:
    std::vector<Signal> m_signals;
    std::vector<Slot> m_slots;
    std::vector<Module*> m_postModules;
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
