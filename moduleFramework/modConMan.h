#ifndef modConMan__h
#define modConMan__h
#include "module.h"


struct signalSlotKey{
  unsigned int ID = 0;
  std::string Name = "";
  bool operator == (const signalSlotKey& otherSignalSlotKey) const
  {
    return(
      this->ID        ==    otherSignalSlotKey.ID   &&
      this->Name  ==    otherSignalSlotKey.Name
    );
  }
  bool operator < (const signalSlotKey& otherSignalSlotKey) const
  {
    return(
      (this->ID <   otherSignalSlotKey.ID)    ||
      (this->ID ==  otherSignalSlotKey.ID   &&  this->Name  <  otherSignalSlotKey.Name)
    );
  }
};
class ModConMan{
public:
  //void init();
protected:
  //void getDelta();
  void putConnectionDataToCache();
  void createAllModules();
  void createModules(const std::map<unsigned int, unsigned int>& constructionMap);//first:ID, second:ModuleType
  void makeConnectionsFromCache();
  void makeConnection(mySqlSignal, signalSlotKey);
  void makeConnection(signalSlotKey, signalSlotKey);
  void makeConnection(signalSlotKey, mySqlSignal);
  std::map<unsigned int, Module*> m_modulesMap;
  std::map<mySqlSignal, signalSlotKey> m_routingInMap;
  std::map<signalSlotKey, signalSlotKey> m_routingInternalMap;
  std::map<signalSlotKey, mySqlSignal> m_routingOutMap;
  mysqlcon* mySqlConnection = nullptr;

};
#endif
