#include <iostream>
#include "ArduIoInterface.h"
#include <string>
#include <list>
#include <mysql/mysql.h>
void ArduIoInterface::serialDispatcher(std::string cmd)
{
  std::cout << "ArduIoInterface::SerialDispatcher" << std::endl;
}
bool ArduIoInterface::connect()
{
  if(serialCmdInterface::connect() && mysqlcon::connect()){
    return true;
  }
  return false;
}
ArduIoInterface::ArduIoInterface(std::string device, int baudrate, std::string host, unsigned int port, std::string user, std::string pw, std::string db) :
                              serialCmdInterface(device, baudrate),
                              mysqlcon(host, port, user, pw, db)
{
  std::cout << "ArduIoInterface::ArduIoInterface" << std::endl;
}
void ArduIoInterface::mainloop()
{
  while(1){
    sendConfig();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
}
bool ArduIoInterface::sendConfig()
{
  std::string sqlQuery = "Select SignalID as PIN, config as cnf from heizung.IoConfigValue Where DeviceID = \"";
  sqlQuery.append(device);
  sqlQuery.append("\";");
  /*
  MYSQL_RES* result = sendCommand(sqlQuery);
  foreach(string[] row in resultList){
    if()
  }
  mysql_free_result(result);
  */
  std::cout << sqlQuery <<std::endl;
}
