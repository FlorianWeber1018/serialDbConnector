#include <iostream>
#include "ArduIoInterface.h"
#include <string>
#include <list>
#include <vector>
#include <mysql/mysql.h>
#include <iomanip>
void ArduIoInterface::serialDispatcher(std::string cmd)
{

  std::cout << "ArduIoInterface::SerialDispatcher:";
  plotFlushStringToConsole(cmd);
  std::cout << std::endl;

  unsigned char cmdByte = cmd[0];
  std::string numberStr = cmd.substr(1);

  std::string sqlQuery="UPDATE ";
  std::string port;
  std::string pin;
  std::string portType;

  if(cmdByte >= setVA0 && cmdByte <= setVA15){
    sqlQuery.append("IoValue SET actualState = ");
    sqlQuery.append(std::to_string(to_short(numberStr)));
    port =  std::to_string((cmdByte-setVA0)/8);
    pin = std::to_string((cmdByte-setVA0)%8);
    portType = "A";
  }else{
    if(cmdByte >= setCA0 && cmdByte <= setCA15){
      sqlQuery.append("IoConfigValue SET actualConfig = ");
      sqlQuery.append(std::to_string(to_uchar(numberStr)));
      port =  std::to_string((cmdByte-setCA0)/8);
      pin = std::to_string((cmdByte-setCA0)%8);
      portType = "A";
    }else{
      if(cmdByte >= setVI0 && cmdByte <= setVI39){
        sqlQuery.append("IoValue SET actualState = ");
        sqlQuery.append(std::to_string(to_uchar(numberStr)));
        port =  std::to_string((cmdByte-setVI0)/8);
        pin = std::to_string((cmdByte-setVI0)%8);
        portType = "I";
      }else{
        if(cmdByte >= setCI0 && cmdByte <= setCI39){
          sqlQuery.append("IoConfigValue SET actualConfig = ");
          sqlQuery.append(std::to_string(to_uchar(numberStr)));
          port =  std::to_string((cmdByte-setCI0)/8);
          pin = std::to_string((cmdByte-setCI0)%8);
          portType = "I";
        }
      }
    }
  }
  sqlQuery.append(" WHERE DeviceID = \'");
  sqlQuery.append(device);
  sqlQuery.append("\' AND PortType = \'");
  sqlQuery.append( portType );
  sqlQuery.append("\' AND Port = \'");
  sqlQuery.append( port );
  sqlQuery.append("\' AND Pin = \'");
  sqlQuery.append( pin );
  sqlQuery.append("\';");

  //std::cout << "ArduIoInterface::serialDispatcher::sqlQuery=" << sqlQuery << std::endl;

  MYSQL_RES* result = sendCommand_dispatcherThread(sqlQuery);
  if(result != NULL){
    mysql_free_result(result);
    std::cout<<"freed result!";
  }

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
    getInput();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

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
  MYSQL_RES* result = sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;
  //std::cout << colCnt <<std::endl;
  while(row = mysql_fetch_row(result)){
    //std::cout << "test" << std::endl;
    std::string flushStr;
    unsigned char pin = std::stol(row[2]) + (std::stol(row[1]) * 8);
    unsigned char config = std::stol(row[3]);
    std::string portType = "";
    portType.append(row[0]);
    if(portType == "A"){
      if(pin < 16){
        flushStr="0";
        flushStr[0] = static_cast<char>(setCA0 + pin);
        flushStr.append(to_flushString(config));
      }else{
        //ERROR
      }
    }else{
      if(portType == "I"){
        if(pin < 40){
          flushStr="0";
          flushStr[0] = static_cast<char>(setCI0 + pin);
          flushStr.append(to_flushString(config));
        }
      }else{
        //ERROR
      }
    }
    std::cout << "now flushing:";
    plotFlushStringToConsole(flushStr);
    std::cout << std::endl;

    serialFlush(flushStr);
  }
  mysql_free_result(result);
}
void ArduIoInterface::sendOutput(bool sendAll)
{
  std::string sqlQuery="Select IoValue.Port, IoValue.Pin, IoValue.targetState from IoValue";
  sqlQuery.append(" left join IoConfigValue ON IoConfigValue.DeviceID = IoValue.DeviceID AND IoConfigValue.PortType = IoValue.PortType AND IoConfigValue.Port = IoValue.Port AND IoConfigValue.Pin = IoValue.Pin");
  sqlQuery.append(" WHERE (actualConfig = 2 OR actualConfig = 3 OR actualConfig = 4 OR actualConfig = 5) AND IoValue.PortType = 'I' AND IoValue.DeviceID = \'");
  sqlQuery.append(device);
  if(sendAll){
    sqlQuery.append("\';");
  }else{
    sqlQuery.append("\' AND NOT IoValue.targetState = IoValue.actualState;");
  }


  MYSQL_RES* result = sendCommand_senderThread(sqlQuery);

  MYSQL_ROW row;

  while(row = mysql_fetch_row(result)){
    //std::cout << "test" << std::endl;
    std::string flushStr;
    unsigned char pin = std::stol(row[0]) + (std::stol(row[1]) * 8);
    unsigned char value = std::stol(row[2]);
    if(pin < 40){
        flushStr="0";
        flushStr[0] = static_cast<char>(setVI0 + pin);
        flushStr.append(to_flushString(value));

        std::cout << "now flushing:";
        plotFlushStringToConsole(flushStr);
        std::cout << std::endl;

        serialFlush(flushStr);
    }
  }
  mysql_free_result(result);
}
void ArduIoInterface::getInput()
{
  std::string sqlQuery="Select IoValue.PortType, IoValue.Port, IoValue.Pin from IoValue ";
  sqlQuery.append(" left join IoConfigValue ON IoConfigValue.DeviceID = IoValue.DeviceID AND IoConfigValue.PortType = IoValue.PortType AND IoConfigValue.Port = IoValue.Port AND IoConfigValue.Pin = IoValue.Pin");
  sqlQuery.append(" WHERE ( ( (actualConfig = 0 OR actualConfig = 1) AND IoValue.PortType = 'I') Or IoValue.PortType = 'A' ) AND IoValue.DeviceID = \'");
  sqlQuery.append(device);
  sqlQuery.append("\';");
//  std::cout<<sqlQuery<<std::endl;

  MYSQL_RES* result = sendCommand_senderThread(sqlQuery);
  MYSQL_ROW row;
  while(row = mysql_fetch_row(result)){
    std::string portType = "";
    portType.append(row[0]);
    unsigned char pin = std::stol(row[2]) + (std::stol(row[1]) * 8);
    std::string flushStr = "";
    if(portType == "A"){
      flushStr[0] = getVA0 + pin;
      if(pin > 15){
        break;
      }
    }
    if(portType == "I"){
      flushStr[0] = getVI0 + pin;
      if(pin > 39){
        break;
      }
    }
    std::cout << "now flushing:";
    plotFlushStringToConsole(flushStr);
    std::cout << std::endl;

    serialFlush(flushStr);
  }
  mysql_free_result(result);
}
void ArduIoInterface::test(){
  while(!mysqlcon::connect()){
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  std::cout << "now sending config once!" << std::endl;
  sendConfig(true);
}
