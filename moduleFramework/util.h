#ifndef util__h
#define util__h

#include "signalRouter.h"
#include <algorithm>
#include <vector>
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
template <typename T>
int findMedian(std::vector<T> in){
  int size = in.size();
  std::sort (in.begin(), in.end());
  if(size % 2){   //odd
    return in[ ( size - 1 ) / 2 ];
  }else{          //even
    return ( in[ size / 2 ] + in[ ( size / 2 ) - 1 ] ) / 2;
  }
};


#endif
