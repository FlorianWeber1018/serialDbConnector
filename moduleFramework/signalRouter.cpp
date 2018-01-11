#include "signalRouter.h"
SignalRouter::SignalRouter(
   std::string host, unsigned int port, std::string user, std::string pw,
   std::string db
)
{
  mySqlConnection = new mysqlcon( host, port, user, pw, db );
  while(!mySqlConnection.connect()){
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
}
Signal* SignalRouter::createSignalIfNotexist(mySqlSignal const& extSignal)
{
  if(m_signals.find(extSignal)){

  }else{
    Signal* newSignal = new Signal();
    m_signals[extSignal] = newSignal;
  }
  return m_signalMap[extSignal];
}
Slot* SignalRouter::createSlotIfNotExist(mySqlSignal const& extSignal)
{
  if(m_slots.find(extSignal)){

  }else{
    Slot* newSlot = new Slot();
    m_slots[extSignal] = newSlot;
  }
  return m_slots[extSignal];
}
void SignalRouter::process()
{

}
void SignalRouter::readMySqlSignalsFromDatabase()
{
  std::string sqlQuery="SELECT IoValue.DeviceID, IoValue.PortType, IoValue.Port,
  IoValue.Pin, IoValue.actualState From IoValue;";
  if(debugMode){
    std::cout<<sqlQuery<<std::endl;
  }
  MYSQL_RES* result = sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;

  while(row = mysql_fetch_row(result)){
    mySqlSignal tempSignal;
    tempSignal.DeviceID = row[0];
    tempSignal.PortType = row[1];
    tempSignal.PortType = row[2];
    tempSignal.Pin      = row[3];
    for (auto signal_mysqlsignal : m_signalMap){
      if(signal_mysqlsignal.second == tempSignal){
        Signal* signal = signal_mysqlsignal.first;
        int value = row[4];
        if(signal != nullptr){
          if(value > signal->max){
            signal->value = signal->max;
          }else{
            if(value < signal->min){
              signal->value = signal->min;
            }else{
              signal->value = value;
            }
          }
          for(auto&& slot: signal->slots){
            slot->synced=true;
          }
      }
    }
    for(auto postModule : m_postModules){
      postModule->trigger();
    }

  }
  mysql_free_result(result);


std::string sqlQuery="Select IoValue.PortType, IoValue.Port, IoValue.Pin from IoValue ";
sqlQuery.append(" left join IoConfigValue ON IoConfigValue.DeviceID = IoValue.DeviceID AND IoConfigValue.PortType = IoValue.PortType AND IoConfigValue.Port = IoValue.Port AND IoConfigValue.Pin = IoValue.Pin");
sqlQuery.append(" WHERE ( ( (actualConfig = 0 OR actualConfig = 1) AND IoValue.PortType = 'I') Or IoValue.PortType = 'A' ) AND IoValue.DeviceID = \'");
sqlQuery.append(device);
sqlQuery.append("\';");
//  std::cout<<sqlQuery<<std::endl;
}
void SignalRouter::writeMySqlSignalsToDatabase()
{

}
