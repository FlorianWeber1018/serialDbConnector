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
  for(auto&& keyValPair : m_signalMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Signal* newSignal = new Signal();
  m_signalMap[newSignal] = extSignal;
  return newSignal;
}
Slot* SignalRouter::createSlotIfNotExist(mySqlSignal const& extSignal)
{
  for(auto&& keyValPair : m_slotMap){
    if(keyValPair.second == extSignal){
      return keyValPair.first;
    }
  }
  Slot* newSlot = new Slot();
  m_slotMap[newSlot] = extSignal;
  return newSlot;
}
void SignalRouter::process()
{

}
void SignalRouter::readMySqlSignalsFromDatabase()
{
  std::string sqlQuery="SELECT IoValue.DeviceID, IoValue.PortType, IoValue.Port,
  IoValue.Pin, IoValue.actualState From IoValue;";

//  std::cout<<sqlQuery<<std::endl;


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
