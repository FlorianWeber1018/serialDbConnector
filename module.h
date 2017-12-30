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
    int executionLevel;
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
    virtual void trigger() = 0;
  private:

  protected:
    void emitSignal(int signalNumber, int value);
    int getSignal(int slotNumber);
  };

  void connect(SignalRouterIn* signalRouter, const& mySqlSignal _extSignal, Slot* _Slot);       //done
  void connect(Signal* _Signal, Slot* _Slot);                                                   //done
  void connect(Signal* _Signal, SignalRouterOut* signalRouter, const& mySqlSignal _extSignal);  //done
}




#endif
