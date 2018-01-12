#include "signalRouter.h"


extern ClockDistributer globalClock;
// ____signalRouterIn___________________________________________________________
SignalRouterIn::SignalRouterIn(
   std::string host, unsigned int port, std::string user, std::string pw,
   std::string db
)
{
  mySqlConnection = new mysqlcon( host, port, user, pw, db );
  while(!mySqlConnection->connect()){
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
  globalClock.addDestination(this);
  if(debugMode) std::cout << "SignalRouterIn::SignalRouterIn()" << std::endl;
}

Signal* SignalRouterIn::createSignalIfNotexist(const mySqlSignal& key)
{
  Signal* signal = nullptr;
  try{ signal = m_signals.at(key); }
  catch (const std::out_of_range& oor) {
    signal = new Signal();
    m_signals[key] = signal;
  }
  return signal;
}

void SignalRouterIn::process()
{
  std::string sqlQuery="SELECT IoValue.DeviceID, IoValue.PortType, IoValue.Port, IoValue.Pin, IoValue.actualState From IoValue;";

  MYSQL_RES* result = mySqlConnection->sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;

  if(result != nullptr){
    while(row = mysql_fetch_row(result)){
      mySqlSignal tempSignal;
      tempSignal.DeviceID = row[0];
      tempSignal.PortType = row[1];
      tempSignal.Port     = row[2];
      tempSignal.Pin      = row[3];

      Signal* signal = nullptr;
      try{ signal = m_signals.at(tempSignal); }
      catch (const std::out_of_range& oor) { continue; }
      if(signal != nullptr){
        signal->value = std::stoi(row[4]);

        for(auto&& slot: signal->slots){
          slot->synced=true;
        }
      }
    }
    mysql_free_result(result);

  if(debugMode) std::cout << "signalRouterIn::process()" << std::endl;
}
// ____signalRouterOut__________________________________________________________

SignalRouterOut::SignalRouterOut(
   std::string host, unsigned int port, std::string user, std::string pw,
   std::string db
)
{
  mySqlConnection = new mysqlcon( host, port, user, pw, db );
  while(!mySqlConnection->connect()){
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
  if(debugMode) std::cout << "SignalRouterOut::SignalRouterOut()" << std::endl;
}

Slot* SignalRouterOut::createSlotIfNotexist(const mySqlSignal& key)
{
  Slot* slot = nullptr;
  try{ slot = m_slots.at(key); }
  catch (const std::out_of_range& oor) {
    slot = new Slot();
    m_slots[key] = slot;
  }
  return slot;
}

void SignalRouterOut::process()
{
5e6ufgzu;
}
