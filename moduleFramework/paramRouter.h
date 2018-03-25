#ifndef paramRouter__h
#define paramRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <iostream>
#include <map>

class ParamRouter : public Module {
public:
  ParamRouter(std::string host, unsigned int port, std::string user,
              std::string pw, std::string db);
  int getParamRef(mySqlParam key);
  bool paramExist(mySqlParam key);
  bool IDExist(unsigned int ID);
  unsigned int getNextAvID();

protected:
  mysqlcon *mySqlConnection = nullptr;
  void process() override;
  std::map<mySqlParam, int> completeCnfMap;
}:

#endif
