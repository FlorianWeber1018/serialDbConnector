#include "signalRouter.h"
#include <chrono>
#include <thread>

extern ClockDistributer globalClock;
// ____signalRouterIn___________________________________________________________
SignalRouterIn::SignalRouterIn(
   std::string host, unsigned int port, std::string user, std::string pw,
   std::string db
)
{
  mySqlConnection = new mysqlcon( host, port, user, pw, db );
  while(!mySqlConnection->connect()){
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
  globalClock.addDestination(this);
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

void SignalRouterIn::trigger()
{
  Module::trigger();
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
      tempSignal.PortType = row[2];
      tempSignal.Pin      = row[3];

      Signal* signal = nullptr;
      try{ signal = m_signals.at(tempSignal); }
      catch (const std::out_of_range& oor) { continue; }
      if(signal != nullptr){
        signal->value = row[4];
        for(auto&& slot: signal->slots){
          slot->synced=true;
        }
      }
    }
    mysql_free_result(result);
  }
}
// ____signalRouterOut__________________________________________________________

/*
void SignalRouterOut::writeMySqlSignalsToDatabase()
{

}
*/
