#include "paramRouter.h"

extern ClockDistributer globalClock;

ParamRouter::ParamRouter(std::string host, unsigned int port, std::string user,
                         std::string pw, std::string db) {
  mySqlConnection = new mysqlcon(host, port, user, pw, db);
  while (!mySqlConnection->connect()) {
    std::cout << "ERROR: mysqlcon::connect() failed" << std::endl;
  }
  globalClock.addDestination(this);
  if (debugMode)
    std::cout << "ParamRouter::ParamRouter()" << std::endl;
}
int ParamRouter::getParam(mySqlParam key) { return completeCnfMap[key]; }
void ParamRouter::process() {
  std::string sqlQuery="SELECT ModuleID AS ID, ParamKey AS key,"
  sqlQuery.append(" Param AS value from ModuleConfig order by ModuleID;";

  MYSQL_RES* result = mySqlConnection->sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;
  if(result != nullptr){
    while (row = mysql_fetch_row(result)) {
      mySqlParam keyOfParam;
      keyOfParam.ID = row[0];
      keyOfParam.paramKey = row[1];
      completeCnfMap[keyOfParam] = row[2];
    }
    mysql_free_result(result);
  }
}
