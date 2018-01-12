#include "signalRouter.h"
#include "util.h"

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
  }
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
  if (debugMode) std::cout << "SignalRouterOut::process()"
    << "  NOTHING WILL HAPPEN HERE" << std::endl;
  for(auto&& key_val : m_slots){
    std::string sqlQuery = "UPDATE IoValue SET targetState = ";
    int preVal = *(key_val.second->value);
    moveToBorders( preVal, key_val.second->min, key_val.second->max );
    sqlQuery.append(std::to_string( preVal ) );
    sqlQuery.append(" WHERE DeviceID = ");
    sqlQuery.append( key_val.first.DeviceID );
    sqlQuery.append(" AND PortType = ");
    sqlQuery.append( key_val.first.PortType );
    sqlQuery.append(" AND Port = ");
    sqlQuery.append( key_val.first.Port );
    sqlQuery.append(" AND Pin = ");
    sqlQuery.append( key_val.first.Pin );
    sqlQuery.append(" ;");
    MYSQL_RES* result = nullptr;
    result = mySqlConnection->sendCommand_senderThread(sqlQuery);
    if (result!= nullptr){
      mysql_free_result(result);
    }
  }

}
