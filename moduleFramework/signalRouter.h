#ifndef signalRouter__h
#define signalRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <map>



class SignalRouter : public Module
{
public:
  SignalRouter::SignalRouter(
    std::string device, std::string host, unsigned int port, std::string user,
    std::string pw, std::string db
  );
  Signal* createSignalIfNotexist(mySqlSignal const& extSignal);
  Slot* createSlotIfNotExist(mySqlSignal const& extSignal);
  mysqlcon* mySqlConnection;
private:
  std::map<Signal*, mySqlSignal> m_signalMap;
  std::map<Slot*, mySqlSignal> m_slotMap;
protected:
  void readMySqlSignalsFromDatabase();
  void writeMySqlSignalsToDatabase();
  void process() override;
};

#endif
