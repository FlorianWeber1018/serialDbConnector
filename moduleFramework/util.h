#ifndef util__h
#define util__h

#include "signalRouter.h"



void connect(SignalRouterIn* signalRouter, mySqlSignal const& _extSignal, Module* receiver, Slot* _Slot);       //done
void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot);                                                   //done
void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, mySqlSignal const& _extSignal);  //done

void moveToBorders(float &value, float const& min, float const& max);
#endif
