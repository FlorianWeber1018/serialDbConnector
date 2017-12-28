#ifndef module__h
#define module__h

#include <vector>
#include <string>
namespace SignalSlots{
  void connect(void* sender, const& Signal Signal, void* receiver, const& Slot slot);
  void connect(std::string DeviceID, std::string PortType, std::string Port, std::string Pin, void* signalRouter, void* receiver, const& Slot slot);
  struct Signal
  {
    int value;
    int min;
    int max;
    int executionLevel;
  };
  struct Slot
  {
    int* value;
    int min;
    int max;
    int executionLevel;
  };
  class module
  {
  public:
    vector<Signal> signal;
    vector<Slot> slot;
    void connectToSignal(int slotNumber, Signal signal);
    void connectToSignal(int slotNumber, std::string DeviceID, std::string PortType, std::string Port, std::string Pin);
  private:
  protected:
  };
}




#endif
