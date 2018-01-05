#include "module.h"
#include <iostream>
  void Module::emitSignal(int signalNumber, int value)
  {
    Signal* signal = m_signals[signalNumber];
    if(signal != nullptr){
      if(value > signal->max){
        signal->value = signal->max;
      }else{
        if(value < signal->min){
          signal->value = signal->min;
        }else{
          signal->value = value;
        }
      }
    }
  }

  int Module::getSignal(int slotNumber)
  {
    Slot* slot = m_slots[slotNumber];
    int preRet = 0;
    if(slot != nullptr){
      preRet = *(slot->value);
      if(preRet > slot->max){
        preRet = slot->max;
      }else{
        if(preRet < slot->min){
          preRet= slot->min;
        }
      }
    }
    return preRet;
  }

  void Module::trigger()
  {
    bool allInputsSynced=true;
    for(auto slot : m_slots){
      if(!slot->synced){
        allInputsSynced=false;
      }
    }
    if(allInputsSynced){
      process();
      triggerNext();
    }
  }

  void Module::triggerNext()
  {
    for(auto postModule : m_postModules){
      postModule->trigger();
    }
  }

  void Module::process()
  {
    std::cout << "virtual method: Module::process() that does nothing" << std::endl;
  }



  void connect(SignalRouterIn* signalRouter, mySqlSignal const& _extSignal, Module* receiver, Slot* _Slot)
  {
    connect( signalRouter->createSignalIfNotexist(_extSignal) , receiver, _Slot );
  }

  void connect(Module* sender, Signal* _Signal, Module* receiver, Slot* _Slot)
  {
    _Slot->value = &(_Signal->value);
    _Signal->slots.append(_Slot);
    sender->m_postModules.append(receiver);
  }

  void connect(Module* sender, Signal* _Signal, SignalRouterOut* signalRouter, mySqlSignal const& _extSignal)
  {
    connect( sender, _Signal, signalRouter, signalRouter->createSlotIfNotExist(_extSignal) );
  }
