#include <iostream>
#include "ArduIoInterface.h"
#include <string>
#include <list>
#include <mysql/mysql.h>
void ArduIoInterface::serialDispatcher(std::string cmd)
{
  std::cout << "ArduIoInterface::SerialDispatcher:" << cmd << std::endl;
}
bool ArduIoInterface::connect()
{
  if(serialCmdInterface::connect() && mysqlcon::connect()){
    startSending();
    startListening();
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
  sendOutput(true);
  sendOutput(false);
  while(1){
    //sendConfig(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}
void ArduIoInterface::sendConfig(bool sendAll)
{
  std::string sqlQuery="Select Port, Pin, Config from heizung.IoConfigValue Where DeviceID = \'";
  sqlQuery.append(device);
  if(sendAll){
    sqlQuery.append("\';");
  }else{
    sqlQuery.append("\' AND Update = 1;");
  }

  //sqlQuery.append(device);
  //sqlQuery.append("\';");
  MYSQL_RES* result = sendCommand(sqlQuery);
  int colCnt = mysql_num_fields(result);
  MYSQL_ROW row;
  //std::cout << colCnt <<std::endl;
  while(row = mysql_fetch_row(result)){
    //std::cout << "test" << std::endl;
    std::string flushStr = "io set config ";
    flushStr.append(row[0]);
    flushStr.append(" ");
    int pin_config = std::stoi(row[1])<<8;
    pin_config |= std::stoi(row[2]);
    flushStr.append(std::to_string(pin_config));
    serialFlush(flushStr);
  }
  mysql_free_result(result);
  sqlQuery="UPDATE heizung.IoConfigValue Set Update = 0 Where Update = 1;";
  result = sendCommand(sqlQuery);
  mysql_free_result(result);
}
void ArduIoInterface::sendOutput(bool sendAll)
{
  std::string sqlQuery="Select IoValue.Port, IoValue.Pin, IoValue.state from IoValue";
  sqlQuery.append(" left join IoConfigValue ON IoConfigValue.DeviceID = IoValue.DeviceID AND IoConfigValue.Port = IoValue.Port AND IoConfigValue.Pin = IoValue.Pin");
  sqlQuery.append(" WHERE (Config = 0 OR Config = 1) AND IoValue.DeviceID = \'");
  sqlQuery.append(device);




  if(sendAll){
    sqlQuery.append("\';");
  }else{
    sqlQuery.append("\' AND IoValue.Outdated = TRUE;");
  }
  std::cout << sqlQuery << endl;
  return;

  MYSQL_RES* result = sendCommand(sqlQuery);
  int colCnt = mysql_num_fields(result);
  MYSQL_ROW row;

  while(row = mysql_fetch_row(result)){
    //std::cout << "test" << std::endl;
    std::string flushStr = "io set value ";
    flushStr.append(row[0]);
    flushStr.append(" ");
    if(row[2]=="1"){
      flushStr.append("true");
    }else{
      flushStr.append("false");
    }
    flushStr.append(" ");
    flushStr.append(row[1]);
    serialFlush(flushStr);
  }
  mysql_free_result(result);
}
