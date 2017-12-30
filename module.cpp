#include "module__h"
namespace SignalSlots {
  void Module::emitSignal(int signalNumber, int value)
  {
    Signal* signal = m_signals[signal];
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
      preRet = slot->value*;
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





  void connect(SignalRouterIn* signalRouter, const& mySqlSignal _extSignal, Slot* _Slot)
  {
  connect( signalRouter->createSignalIfNotexist(_extSignal) , _Slot );
  }

  void connect(Signal* _Signal, Slot* _Slot)
  {
    _Slot->value = &(_Signal->value);
    _Signal->slots.append(_Slot);
  }

  void connect(Signal* _Signal, SignalRouterOut* signalRouter, const& mySqlSignal _extSignal)
  {
    connect( _Signal, signalRouter->createSlotIfNotExist(_extSignal) );
  }


}
