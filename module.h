#ifndef module__h
#define module__h

#include <vector>
#include <string>

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



#endif
