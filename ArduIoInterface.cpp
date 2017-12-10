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
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
}
bool ArduIoInterface::sendConfig()
{
  std::string sqlQuery = "Select Port, Pin, Config from heizung.IoConfigValue Where DeviceID = \'";
  sqlQuery.append(device);
  sqlQuery.append("\';");
  std::cout<< sqlQuery<<std::endl;
  MYSQL_RES* result = sendCommand(sqlQuery);
  int colCnt = mysql_num_fields(result);
  MYSQL_ROW row;
  while(row = mysql_fetch_row(result)){
    for(int i = 0; i < colCnt; i++){
      std::string flushStr = "io set config ";
      flushStr.append(row[0]);
      flushStr.append(" ");
      int pin_config = std::stoi(row[1])<<8;
      pin_config |= std::stoi(row[2]);
      flushStr.append(std::to_string(pin_config));
      std::cout<<flushStr<<std::endl;
    }
  }
  mysql_free_result(result);

  //std::cout << sqlQuery <<std::endl;
}
