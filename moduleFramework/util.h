#ifndef util__h
#define util__h

#include "signalRouter.h"

struct Time
{
  int year;
  int month;
  int weekday;
  int day;
  int hour;
  int minute;
  int second;
};
void connectToTime(SignalRouterIn* signalRouter, const std::string& key,
  Module* receiver, Slot* _Slot
);

void connect(SignalRouterIn* signalRouter, mySqlSignal const& _extSignal,
  Module* receiver, Slot* _Slot
);
void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot);
void connect(
  Module* sender, Signal* _Signal,
  SignalRouterOut* signalRouter, mySqlSignal const& _extSignal
);

template <typename T>
void moveToBorders(T &value, T const& min, T const& max)
{
  if(value < min){
    value = min;
  }else if(value > max){
    value = max;
  }
};

template <typename T>
void moveToBorders(
  T &value, T const& min, T const& max, T const& min_value, T const& max_value)
{
  if(value < min){
    value = min_value;
  }else if(value > max){
    value = max_value;
  }
};


#endif
