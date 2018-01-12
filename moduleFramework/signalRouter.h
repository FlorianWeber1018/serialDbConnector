#ifndef signalRouter__h
#define signalRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <map>
#include <iostream>

class SignalRouterIn : public Module
{
public:
  SignalRouterIn(
     std::string host, unsigned int port, std::string user, std::string pw,
     std::string db
  );
  Signal* createSignalIfNotexist(const mySqlSignal& key);


protected:
  mysqlcon* mySqlConnection = nullptr;
  void process() override;
//  std::map<mySqlSignal, Signal*, mySqlSignalCompare> m_signals;
  std::map<mySqlSignal, Signal*> m_signals;
  void emitSignal(const mySqlSignal& key, int value);
private:
};

class SignalRouterOut : public Module
{
public:
  SignalRouterOut(
     std::string host, unsigned int port, std::string user, std::string pw,
     std::string db
  );
  Slot* createSlotIfNotexist(const mySqlSignal& key);

protected:
  mysqlcon* mySqlConnection = nullptr;
  void process() override;
//  std::map<mySqlSignal, Signal*, mySqlSignalCompare> m_signals;
  std::map<mySqlSignal, Slot*> m_slots;
  int getSignalValue(const mySqlSignal& key);
private:
};

#endif
