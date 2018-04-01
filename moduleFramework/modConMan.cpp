#include "module.h"
#include "util.h"
#include <iostream>
#include "modConMan.h"
extern SignalRouterIn* modRouterIn;
extern SignalRouterOut* modRouterOut;

std::map <std::string, unsigned int> mappingType_number;



ModConMan::ModConMan(std::string host, unsigned int port,
                               std::string user, std::string pw,
                               std::string db)
{
  mappingType_number["2Point"]      = 0;
  mappingType_number["3WayValve"]   = 1;
  mappingType_number["constant"]    = 2;
  mappingType_number["debug"]       = 3;
  mappingType_number["Inverter"]    = 4;
  mySqlConnection = new mysqlcon(host, port, user, pw, db);
  while (!mySqlConnection->connect()) {
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
  createAllModules();
  putConnectionDataToCache();
  makeConnectionsFromCache();
}

void ModConMan::putConnectionDataToCache(){
  std::string sqlQuery =
    "SELECT srcDeviceID, srcPortType, srcPort, srcPin, destID, destSlotName";
    //            0           1           2       3       4         5
  sqlQuery.append(" from ModuleRoutingIn;");
  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  MYSQL_ROW row;
  if (result != nullptr) {
    while (row = mysql_fetch_row(result)) {
      mySqlSignal src;
        src.DeviceID = row[0];
        src.PortType = row[1];
        src.Port     = row[2];
        src.Pin      = row[3];
      signalSlotKey dest;
        dest.ID      = std::stoul(row[4]);
        dest.Name    = row[5];
      m_routingInMap[src] = dest;
    }
    mysql_free_result(result);
  }

  sqlQuery =
    "SELECT srcID, srcSignalName, destID, destSlotName";
    //        0          1           2          3
  sqlQuery.append(" from ModuleRoutingInternal;");
  result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  if (result != nullptr) {
    while (row = mysql_fetch_row(result)) {
      signalSlotKey src;
        src.ID  = std::stoul(row[0]);
        src.Name = row[1];
      signalSlotKey dest;
        dest.ID = std::stoul(row[2]);
        dest.Name = row[3];
      m_routingInternalMap[src] = dest;
    }
    mysql_free_result(result);
  }

  sqlQuery =
    "SELECT srcID, srcSignalName, destDeviceID, destPortType, destPort, destPin";
    //        0          1              2             3           4         5
  sqlQuery.append(" from ModuleRoutingOut;");
  result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  if (result != nullptr) {
    while (row = mysql_fetch_row(result)) {
      signalSlotKey src;
        src.ID  = stoul(row[0]);
        src.Name = row[1];
      mySqlSignal dest;
        dest.DeviceID = row[2];
        dest.PortType = row[3];
        dest.Port     = row[4];
        dest.Pin      = row[5];
      m_routingInternalMap[src] = dest;
    }
    mysql_free_result(result);
  }
}
void ModConMan::createAllModules(){
  std::map<unsigned int, unsigned int> constructionMap;
  std::string sqlQuery = "SELECT ID, Type from ModuleList;";

  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;
  if (result != nullptr) {
    while (row = mysql_fetch_row(result)) {
      try{
        constructionMap[std::stoul(row[0])] = mappingType_number.at(row[1]);
      }
      catch (const std::exception &e) {
        if (debugMode) {
          std::cout << "exception was cought : " << e.what() << std::endl;
        }
        std::cout<< "ERROR ModuleType not in case of translationMap" << std::endl;
        return;
      }
    }
    createModules(constructionMap);
    mysql_free_result(result);
  }
}
void ModConMan::createModules(const std::map<unsigned int, unsigned int>& constructionMap)
{
  for(auto&& ID_Type : constructionMap){
    switch(ID_Type.second){
      case 0:{
        m_modulesMap[ID_Type.first] = new Module_2Point(ID_Type.first);
      } break;
      case 1:{
        m_modulesMap[ID_Type.first] = new Module_3WayValve(ID_Type.first);
      } break;
      case 2:{
        m_modulesMap[ID_Type.first] = new Module_constant(ID_Type.first);
      } break;
      case 3:{
        m_modulesMap[ID_Type.first] = new Module_debug(ID_Type.first);
      } break;
      case 4:{
        m_modulesMap[ID_Type.first] = new Module_Inverter(ID_Type.first);
      } break;
      default:{
        std::cout<< "ERROR ModuleType not in case" << std::endl;
      }
    }
  }
}
void ModConMan::makeConnectionsFromCache(){
  for(auto&& mySqlSig_SignalSlotKey : m_routingInMap){
    makeConnection(mySqlSig_SignalSlotKey.first, mySqlSig_SignalSlotKey.second);
  }
  for(auto&& SignalSlotKey_SignalSlotKey : m_routingInternalMap){
    makeConnection(SignalSlotKey_SignalSlotKey.first,  SignalSlotKey_SignalSlotKey.second);
  }
  for(auto&& SignalSlotKey_mySqlSig : m_routingOutMap){
    makeConnection(SignalSlotKey_mySqlSig.first, SignalSlotKey_mySqlSig.second);
  }
}
void ModConMan::makeConnection(
  mySqlSignal _mySqlSignal, signalSlotKey _signalSlotKey)
{
  Module* receiver = nullptr;
  Slot* slot = nullptr;
  try {
    receiver  = m_modulesMap.at(_signalSlotKey.ID);
    slot      = receiver->getSlot(_signalSlotKey.Name);
  }
  catch (const std::exception &e) {
    if (debugMode) {
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return;
  }
  if( (slot != nullptr) && (receiver != nullptr) ){
    connect(modRouterIn, _mySqlSignal, receiver, slot);
  }
}
void ModConMan::makeConnection(
  signalSlotKey _signalSlotKeySender, signalSlotKey _signalSlotKeyReceiver)
{
  Module* sender = nullptr;
  Signal* signal = nullptr;
  Module* receiver = nullptr;
  Slot* slot = nullptr;
  try {
    sender    = m_modulesMap.at(_signalSlotKeySender.ID);
    signal    = sender->getSignal(_signalSlotKeySender.Name);
    receiver  = m_modulesMap.at(_signalSlotKeyReceiver.ID);
    slot      = receiver->getSlot(_signalSlotKeyReceiver.Name);
  }
  catch (const std::exception &e) {
    if (debugMode) {
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return;
  }
  if( (signal != nullptr) && (sender != nullptr) &&
      (slot != nullptr) && (receiver != nullptr) ){
    connect(sender, signal, receiver, slot);
  }
}
void ModConMan::makeConnection(
  signalSlotKey _signalSlotKey, mySqlSignal _mySqlSignal )
{
  Module* sender = nullptr;
  Signal* signal = nullptr;

  try {
    sender    = m_modulesMap.at(_signalSlotKey.ID);
    signal    = sender->getSignal(_signalSlotKey.Name);
  }
  catch (const std::exception &e) {
    if (debugMode) {
      std::cout << "exception was cought : " << e.what() << std::endl;
    }
    return;
  }
  if( (signal != nullptr) && (sender != nullptr) ){
    connect(sender, signal, modRouterOut, _mySqlSignal);
  }
}
