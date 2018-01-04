#ifndef util__h
#define util__h
#include <string>
#include <vector>


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
  std::vector<Slot*> slots;
};

/* 
void connect(SignalRouterIn* signalRouter, const& mySqlSignal _extSignal, Module* receiver, Slot* _Slot);       //done
void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot);                                                   //done
void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, const& mySqlSignal _extSignal);  //done
*/
#endif
