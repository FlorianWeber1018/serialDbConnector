#ifndef module__h
#define module__h

#include <vector>
#include <string>
#include "util.h"


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



#endif
