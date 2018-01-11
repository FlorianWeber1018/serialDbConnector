#ifndef signalRouter__h
#define signalRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <map>



class SignalRouter : public Module
{
public:

  Signal* createSignalIfNotexist(mySqlSignal const& extSignal);
  Slot* createSlotIfNotExist(mySqlSignal const& extSignal);
  mysqlcon* mySqlConnection;
private:
  std::map<Signal*, mySqlSignal> m_signalMap;
  std::map<Slot*, mySqlSignal> m_slotMap;
protected:
  void process() override;
};

#endif
