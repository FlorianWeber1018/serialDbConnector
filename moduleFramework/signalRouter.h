#ifndef signalRouter__h
#define signalRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <map>
#include <iostream>

class SignalRouterIn : protected Module
{
public:
  SignalRouterIn(
     std::string host, unsigned int port, std::string user, std::string pw,
     std::string db
  );
  Signal* createSignalIfNotexist(const mySqlSignal& key);
  void trigger();

protected:
  mysqlcon* mySqlConnection = nullptr;
  void process() override;
  std::map<mySqlSignal, Signal*, mySqlSignalCompare> m_signals;
  void emitSignal(const mySqlSignal& key, int value);
private:
};

#endif
