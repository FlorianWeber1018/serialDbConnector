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

  m_timeSignals["year"] = new Signal();
  m_timeSignals["month"] = new Signal();
  m_timeSignals["day"] = new Signal();
  m_timeSignals["weekday"] = new Signal();
  m_timeSignals["hour"] = new Signal();
  m_timeSignals["minute"] = new Signal();
  m_timeSignals["second"] = new Signal();

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
  //getTime();
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

Signal* SignalRouterIn::getTimeSignal(const std::string& key)
{
  Signal* signal = nullptr;
  try{ signal = m_timeSignals.at(key); }
  catch (const std::out_of_range& oor) { return nullptr; }
  return signal;
}
void SignalRouterIn::getTime()
{
  std::string sqlQuery="SELECT NOW();";
  MYSQL_RES* result = mySqlConnection->sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;
  std::string now = "";
  if(result != nullptr){
    now = row[0];
    mysql_free_result(result);
  }else{
    return;
  }

  for (auto&& signalName_signal : m_timeSignals){
    sqlQuery="SELECT ";
    sqlQuery.append(signalName_signal.first);
    sqlQuery.append("('");
    sqlQuery.append(now);
    sqlQuery.append("');");
    result = mySqlConnection->sendCommand_senderThread(sqlQuery);
    if(result != nullptr){
      if(signalName_signal.second != nullptr){
        signalName_signal.second->value = std::stoi(row[0]);
        for(auto&& slot: signalName_signal.second->slots){
          slot->synced=true;
        }
      }
      mysql_free_result(result);
    }
  }

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
  for(auto&& key_val : m_slots){
    std::string sqlQuery = "UPDATE IoValue SET targetState = ";
    int preVal = *(key_val.second->value);
    moveToBorders( preVal, key_val.second->min, key_val.second->max );
    sqlQuery.append(std::to_string( preVal ) );
    sqlQuery.append(" WHERE DeviceID = '");
    sqlQuery.append( key_val.first.DeviceID );
    sqlQuery.append("' AND PortType = '");
    sqlQuery.append( key_val.first.PortType );
    sqlQuery.append("' AND Port = '");
    sqlQuery.append( key_val.first.Port );
    sqlQuery.append("' AND Pin = '");
    sqlQuery.append( key_val.first.Pin );
    sqlQuery.append("' ;");
    MYSQL_RES* result = nullptr;
    //std::cout << sqlQuery << std::endl;
    result = mySqlConnection->sendCommand_senderThread(sqlQuery);
    if (result!= nullptr){
      mysql_free_result(result);
    }
  }

}
