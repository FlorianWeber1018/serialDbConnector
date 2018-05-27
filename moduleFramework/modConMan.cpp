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
  mappingType_number["2Point"]          = 0;
  mappingType_number["3WayValve"]       = 1;
  mappingType_number["constant"]        = 2;
  mappingType_number["debug"]           = 3;
  mappingType_number["inverter"]        = 4;
  mappingType_number["medianFilter"]    = 5;
  mySqlConnection = new mysqlcon(host, port, user, pw, db);
  while (!mySqlConnection->connect()) {
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
  createAllModules();
  putConnectionDataToCache();
  makeConnectionsFromCache();
  if (debugMode)
    std::cout << "ModConMan::ModConMan()" << std::endl;
}

void ModConMan::putConnectionDataToCache(){
  if(debugMode==2){
    std::cout << "ModConMan::putConnectionDataToCache()" << std::endl;
  }
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
      m_routingInMap[dest] = src;
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
      m_routingInternalMap[dest] = src;
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
        src.ID  = std::stoul(row[0]);
        src.Name = row[1];
      mySqlSignal dest;
        dest.DeviceID = row[2];
        dest.PortType = row[3];
        dest.Port     = row[4];
        dest.Pin      = row[5];
      m_routingOutMap[dest] = src;
    }
    mysql_free_result(result);
  }
}
void ModConMan::createAllModules(){
  if(debugMode==2){
    std::cout << "ModConMan::createAllModules()" << std::endl;
  }
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
void ModConMan::createModules(const std::map<unsigned int, unsigned int>& constructionMap){
  if(debugMode==2){
    std::cout << "ModConMan::createModules()" << std::endl;
  }
  for(auto&& ID_Type : constructionMap){
    if(debugMode==2){
      std::cout << "ID:" << ID_Type.first << " Type:" << ID_Type.second << std::endl;
    }
    switch(ID_Type.second){
      case 0:{
        if(debugMode==2){
          std::cout << "new Module_2Point will be created with ID: " << ID_Type.first << std::endl;
        }
        m_modulesMap[ID_Type.first] = new Module_2Point(ID_Type.first);
        if(debugMode==2){
          std::cout << "new Module_2Point was created with ID: " << ID_Type.first << std::endl;
        }
      } break;
      case 1:{
        if(debugMode==2){
          std::cout << "new Module_3WayValve will be created with ID: " << ID_Type.first << std::endl;
        }
        m_modulesMap[ID_Type.first] = new Module_3WayValve(ID_Type.first);
        if(debugMode==2){
          std::cout << "new Module_3WayValve was created with ID: " << ID_Type.first << std::endl;
        }
      } break;
      case 2:{
        if(debugMode==2){
          std::cout << "new Module_constant will be created with ID: " << ID_Type.first << std::endl;
        }
        m_modulesMap[ID_Type.first] = new Module_constant(ID_Type.first);
        if(debugMode==2){
          std::cout << "new Module_constant was created with ID: " << ID_Type.first << std::endl;
        }
      } break;
      case 3:{
        if(debugMode==2){
          std::cout << "new Module_debug will be created with ID: " << ID_Type.first << std::endl;
        }
        m_modulesMap[ID_Type.first] = new Module_debug(ID_Type.first);
        if(debugMode==2){
          std::cout << "new Module_debug was created with ID: " << ID_Type.first << std::endl;
        }
      } break;
      case 4:{
        if(debugMode==2){
          std::cout << "new Module_Inverter will be created with ID: " << ID_Type.first << std::endl;
        }
        m_modulesMap[ID_Type.first] = new Module_Inverter(ID_Type.first);
        if(debugMode==2){
          std::cout << "new Module_Inverter was created with ID: " << ID_Type.first << std::endl;
        }
      } break;
      case 5:{
        if(debugMode==2){
          std::cout << "new Module_MedianFilter will be created with ID: " << ID_Type.first << std::endl;
        }
        m_modulesMap[ID_Type.first] = new Module_MedianFilter(ID_Type.first);
        if(debugMode==2){
          std::cout << "new Module_MedianFilter was created with ID: " << ID_Type.first << std::endl;
        }
      } break;
      default:{
        std::cout<< "ERROR ModuleType not in case" << std::endl;
      }
    }
  }
  if(debugMode == 2){
    for(auto&& ID_Module : m_modulesMap){
      std::cout << "m_modulesMap.key:" << ID_Module.first << " m_modulesMap.value:" << ID_Module.second << std::endl;
    }
  }

}

void ModConMan::makeConnectionsFromCache(){
  if(debugMode==2){
    std::cout << "ModConMan::makeConnectionsFromCache()" << std::endl;
  }
  for(auto&& mySqlSig_SignalSlotKey : m_routingInMap){
    if(debugMode==2){
      std::cout << "connect: " << mySqlSig_SignalSlotKey.second.DeviceID << ", ";
      std::cout << mySqlSig_SignalSlotKey.second.PortType << ", ";
      std::cout << mySqlSig_SignalSlotKey.second.Port << ", ";
      std::cout << mySqlSig_SignalSlotKey.second.Pin << " -> ";
      std::cout << mySqlSig_SignalSlotKey.first.ID << ".";
      std::cout << mySqlSig_SignalSlotKey.first.Name << " | CALL"<< std::endl;
    }
    makeConnection(mySqlSig_SignalSlotKey.second, mySqlSig_SignalSlotKey.first);
    if(1){
      std::cout << "connect: " << mySqlSig_SignalSlotKey.second.DeviceID << ", ";
      std::cout << mySqlSig_SignalSlotKey.second.PortType << ", ";
      std::cout << mySqlSig_SignalSlotKey.second.Port << ", ";
      std::cout << mySqlSig_SignalSlotKey.second.Pin << " -> ";
      std::cout << mySqlSig_SignalSlotKey.first.ID << ".";
      std::cout << mySqlSig_SignalSlotKey.first.Name << " | DONE"<< std::endl;
    }
  }
  for(auto&& SignalSlotKey_SignalSlotKey : m_routingInternalMap){
    if(debugMode==2){
      std::cout << "connect: " << SignalSlotKey_SignalSlotKey.second.ID << ".";
      std::cout << SignalSlotKey_SignalSlotKey.second.Name << " -> ";
      std::cout << SignalSlotKey_SignalSlotKey.second.ID << ".";
      std::cout << SignalSlotKey_SignalSlotKey.second.Name << " | CALL"<< std::endl;
    }
    makeConnection(SignalSlotKey_SignalSlotKey.second,  SignalSlotKey_SignalSlotKey.first);
    if(1){
      std::cout << "connect: " << SignalSlotKey_SignalSlotKey.second.ID << ".";
      std::cout << SignalSlotKey_SignalSlotKey.second.Name << " -> ";
      std::cout << SignalSlotKey_SignalSlotKey.first.ID << ".";
      std::cout << SignalSlotKey_SignalSlotKey.first.Name << " | DONE"<< std::endl;
    }
  }
  for(auto&& SignalSlotKey_mySqlSig : m_routingOutMap){
    if(debugMode==2){
      std::cout << "connect: " << SignalSlotKey_mySqlSig.second.ID << ".";
      std::cout << SignalSlotKey_mySqlSig.second.Name << " -> ";
      std::cout << SignalSlotKey_mySqlSig.first.DeviceID << ", ";
      std::cout << SignalSlotKey_mySqlSig.first.PortType << ", ";
      std::cout << SignalSlotKey_mySqlSig.first.Port << ", ";
      std::cout << SignalSlotKey_mySqlSig.first.Pin << " | CALL"<< std::endl;
    }
    makeConnection(SignalSlotKey_mySqlSig.second, SignalSlotKey_mySqlSig.first);
    if(1){
      std::cout << "connect: " << SignalSlotKey_mySqlSig.second.ID << ".";
      std::cout << SignalSlotKey_mySqlSig.second.Name << " -> ";
      std::cout << SignalSlotKey_mySqlSig.first.DeviceID << ", ";
      std::cout << SignalSlotKey_mySqlSig.first.PortType << ", ";
      std::cout << SignalSlotKey_mySqlSig.first.Port << ", ";
      std::cout << SignalSlotKey_mySqlSig.first.Pin << " | DONE"<< std::endl;
    }
  }
}
void ModConMan::makeConnection(
  mySqlSignal _mySqlSignal, signalSlotKey _signalSlotKey)
{
  if(debugMode==2){
    std::cout << "ModConMan::makeConnection()" << std::endl;
  }
  Module* receiver = nullptr;
  Slot* slot = nullptr;
  try {
    if(debugMode==2){
      std::cout << "search for receiver at ID: " << _signalSlotKey.ID << std::endl;
    }
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
    if(debugMode==2){
      std::cout << "connect input to module" << std::endl;
    }
  }
}
void ModConMan::makeConnection(
  signalSlotKey _signalSlotKeySender, signalSlotKey _signalSlotKeyReceiver)
{
  if(debugMode==2){
    std::cout << "ModConMan::makeConnection()" << std::endl;
  }
  Module* sender = nullptr;
  Signal* signal = nullptr;
  Module* receiver = nullptr;
  Slot* slot = nullptr;
  try {
    if(debugMode==2){
      std::cout << "search for sender at ID: " << _signalSlotKeySender.ID;
      std::cout << "and for receiver at ID: " << _signalSlotKeyReceiver.ID << std::endl;
    }
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
    if(debugMode==2){
      std::cout << "connect internal" << std::endl;
    }
  }
}
void ModConMan::makeConnection(
  signalSlotKey _signalSlotKey, mySqlSignal _mySqlSignal )
{
  if(debugMode==2){
    std::cout << "ModConMan::makeConnection()" << std::endl;
  }
  Module* sender = nullptr;
  Signal* signal = nullptr;

  try {
    if(debugMode==2){
      std::cout << "search for sender at ID: " << _signalSlotKey.ID;
      std::cout << " And get ID with Name: " << _signalSlotKey.Name << std::endl;
    }
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
    if(debugMode==2){
      std::cout << "connect module to output" << std::endl;
    }
  }
}
