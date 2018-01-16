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
  Signal* getTimeSignal(const std::string& key);

protected:
  mysqlcon* mySqlConnection = nullptr;
  void process() override;
  void getTime();
//  std::map<mySqlSignal, Signal*, mySqlSignalCompare> m_signals;
  std::map<mySqlSignal, Signal*> m_signals;
  std::map<string, Signal*> m_timeSignals;


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
private:
};

#endif
