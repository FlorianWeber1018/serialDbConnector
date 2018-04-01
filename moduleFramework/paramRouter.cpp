#include "paramRouter.h"

extern ClockDistributer globalClock;
extern ParamRouter* globalParams;

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
  std::string sqlQuery = "SELECT ModuleID AS ID, ParamKey AS key,";
  sqlQuery.append(" Param AS value from ModuleConfig order by ModuleID;");

  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;
  if (result != nullptr) {
    while (row = mysql_fetch_row(result)) {
      mySqlParam keyOfParam;
      keyOfParam.ID = std::stoul(row[0]);
      keyOfParam.paramKey = row[1];
      completeCnfMap[keyOfParam] = std::stoi(row[2]);
    }
    mysql_free_result(result);
  }
  if (debugMode)
    std::cout << "ParamRouter::process()" << std::endl;
  for(auto&& paramKey_param : completeCnfMap){
    std::cout << paramKey_param.first.ID << "-" << paramKey_param.first.paramKey;
    std::cout << "=" << paramKey_param.second << std::endl;
  }
}
bool ParamRouter::paramExist(mySqlParam key) {
  bool ret = false;
  std::string sqlQuery = "SELECT * from ModuleConfig where ModuleID = ";
  sqlQuery.append(std::to_string(key.ID));
  sqlQuery.append(" AND ParamKey = '");
  sqlQuery.append(key.paramKey);
  sqlQuery.append("' LIMIT 1;");

  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  if (result != nullptr) {
    if (mysql_fetch_row(result)) {
      ret = true;
    } else {
      ret = false;
    }
    mysql_free_result(result);
  }
  return ret;
}
bool ParamRouter::IDExist(unsigned int ID) {
  bool ret = false;
  std::string sqlQuery = "SELECT * from ModuleConfig where ModuleID = ";
  sqlQuery.append(std::to_string(ID));
  sqlQuery.append(" LIMIT 1;");

  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  if (result != nullptr) {
    if (mysql_fetch_row(result)) {
      ret = true;
    } else {
      ret = false;
    }
    mysql_free_result(result);
  }
  return ret;
}
unsigned int ParamRouter::getNextAvID() {
  unsigned int nextAvID = UINT_MAX;
  std::string sqlQuery =
      "SELECT ModuleID from ModuleConfig Order By ModuleID DESC LIMIT 1;";
  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  MYSQL_ROW row;
  if (result != nullptr) {
    if (row = mysql_fetch_row(result)) {
      nextAvID = std::stoul(row[0]) + 1;
    } else {
      nextAvID = 1;
    }
    mysql_free_result(result);
  }
  return nextAvID;
}
void ParamRouter::createParam(mySqlParam key, unsigned int defaultParam) {
  std::string sqlQuery = "INSERT INTO ModuleConfig (ModuleID, ParamKey, Param)";
  sqlQuery.append(" VALUES (");
  sqlQuery.append(std::to_string(key.ID));
  sqlQuery.append(", '");
  sqlQuery.append(key.paramKey);
  sqlQuery.append("', ");
  sqlQuery.append(std::to_string(defaultParam));
  sqlQuery.append(");");
  MYSQL_RES *result = mySqlConnection->sendCommand_senderThread(sqlQuery);
  if (result != nullptr) {
    mysql_free_result(result);
  }else{

  }
}
void ParamRouter::createParamIfNotExist(mySqlParam key, unsigned int defaultParam) {
  if(!paramExist(key)){
    createParam(key, defaultParam);
  }
}
