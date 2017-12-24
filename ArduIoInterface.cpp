#include <iostream>
#include "ArduIoInterface.h"
#include <string>
#include <list>
#include <mysql/mysql.h>
void ArduIoInterface::serialDispatcher(std::string cmd)
{
  std::cout << "ArduIoInterface::SerialDispatcher:" << cmd << std::endl;
  std::string sqlQuery="UPDATE";
  if(cmd[0]=='V'){
    sqlQuery.append("IoValue SET actualState = ");
    sqlQuery.append(cmd.substr(7 , (cmd.length()-1) );
  }else if(cmd[0]=='C'){
    sqlQuery.append("IoConfigValue SET actualConfig = ");
    sqlQuery.append(cmd.substr(7 , (cmd.length()-1) );
  }
  sqlQuery.append(" WHERE DeviceID = ");
  sqlQuery.append(device);
  sqlQuery.append(" AND PortType = ");
  sqlQuery.append(cmd[2]);
  sqlQuery.append(" AND Port = ");
  sqlQuery.append(cmd[4);
  sqlQuery.append(" AND Pin = ");
  sqlQuery.append(cmd[5);
  std::cout << "ArduIoInterface::sqlQuery=" << sqlQuery << std::endl;
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

  sendConfig(true);
  sendOutput(true);
  while(1){
    sendConfig(false);
    sendOutput(false);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  }
}
void ArduIoInterface::sendConfig(bool sendAll)
{
  std::string sqlQuery="Select PortType, Port, Pin, targetConfig from heizung.IoConfigValue Where DeviceID = \'";
  sqlQuery.append(device);
  if(sendAll){
    sqlQuery.append("\';");
  }else{
    sqlQuery.append("\' AND NOT targetConfig = actualConfig;");
  }
  MYSQL_RES* result = sendCommand(sqlQuery);
  int colCnt = mysql_num_fields(result);
  MYSQL_ROW row;
  //std::cout << colCnt <<std::endl;
  while(row = mysql_fetch_row(result)){
    //std::cout << "test" << std::endl;
    std::string flushStr = "S C ";
    flushStr.append(row[0]);
    flushStr.append(" ");
    flushStr.append(row[1]);
    flushStr.append(row[2]);
    flushStr.append(" ");
    flushStr.append(row[3]);
    serialFlush(flushStr);
    std::cout<<flushStr<<std::endl;
  }
  mysql_free_result(result);
}
void ArduIoInterface::sendOutput(bool sendAll)
{
  std::string sqlQuery="Select IoValue.Port, IoValue.Pin, IoValue.targetState from IoValue";
  sqlQuery.append(" left join IoConfigValue ON IoConfigValue.DeviceID = IoValue.DeviceID AND IoConfigValue.PortType = IoValue.PortType AND IoConfigValue.Port = IoValue.Port AND IoConfigValue.Pin = IoValue.Pin");
  sqlQuery.append(" WHERE (actualConfig = 2 OR actualConfig = 3) AND IoValue.PortType = 'I' AND IoValue.DeviceID = \'");
  sqlQuery.append(device);
  if(sendAll){
    sqlQuery.append("\';");
  }else{
    sqlQuery.append("\' AND NOT IoValue.targetState = IoValue.actualState;");
  }


  MYSQL_RES* result = sendCommand(sqlQuery);
  int colCnt = mysql_num_fields(result);
  MYSQL_ROW row;

  while(row = mysql_fetch_row(result)){
    //std::cout << "test" << std::endl;
    std::string flushStr = "S V I ";
    flushStr.append(row[0]);
    flushStr.append(row[1]);
    flushStr.append(" ");
    flushStr.append(row[2]);
    serialFlush(flushStr);
    std::cout<<flushStr<<std::endl;
  }
  mysql_free_result(result);
}
