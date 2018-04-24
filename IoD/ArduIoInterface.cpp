#include <iostream>
#include "ArduIoInterface.h"
#include <string>
#include <list>
#include <vector>
#include <mysql/mysql.h>
#include <iomanip>
void ArduIoInterface::serialDispatcher(std::string cmd)
{
  std::cout << "ArduIoInterface::SerialDispatcher:" << cmd << std::endl;
  std::string sqlQuery="UPDATE ";
  std::vector<std::string> cmdVector;

  std::string::size_type i = 0;
  std::string::size_type j = cmd.find(' ');
  if (j == std::string::npos){
    return;
  }
  //std::cout << "ArduIoInterface::serialDispatcher: \' \' found at:" << j << std::endl;
  while (j != std::string::npos) {
      cmdVector.push_back(cmd.substr(i, j-i));
      i = ++j;
      j = cmd.find(' ', j);
      //std::cout << "ArduIoInterface::serialDispatcher(innerloop): \' \' found at:" << j << std::endl;
      if (j == std::string::npos){
        cmdVector.push_back(cmd.substr(i, cmd.length()));
      }
  }
//  for(string element : cmdVector){
//    std::cout << element << std::endl;
//  }
  if(cmdVector[0]=="V"){
    sqlQuery.append("IoValue SET actualState = ");
    sqlQuery.append(cmdVector[3]);
  }else if(cmdVector[0]=="C"){
    sqlQuery.append("IoConfigValue SET actualConfig = ");
    sqlQuery.append(cmdVector[3]);
  }
  sqlQuery.append(" WHERE DeviceID = \'");
  sqlQuery.append(device);
  sqlQuery.append("\' AND PortType = \'");
  sqlQuery.append( cmdVector[1] );
  sqlQuery.append("\' AND Port = \'");
  sqlQuery.append( std::to_string(std::stoi(cmdVector[2])/10) );
  sqlQuery.append("\' AND Pin = \'");
  sqlQuery.append( std::to_string(std::stoi(cmdVector[2])%10) );
  sqlQuery.append("\';");
  //std::cout << "ArduIoInterface::sqlQuery=" << sqlQuery << std::endl;
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
    unsigned int pin = std::stol(row[2]) + (std::stol(row[1]) * 8);
    unsigned char config = std::stol(row[3]);
    std::cout << "pin:" << pin <<" PortType:"<<row[0]<<std::endl;
    if(row[0] == "A"){
      std::cout << "A!" << '\n';
      if(pin < 16){
        flushStr="0";
        flushStr[0] = static_cast<char>(setCA0 + pin);
        flushStr.append(to_flushString(config));
        std::cout << "flushStr.length()" << flushStr.length() << std::endl;
      }else{
        //ERROR
      }
    }else{
      std::cout << "not A :(" << '\n';
      if(row[0] == "I"){
        if(pin < 40){
          flushStr="0";
          flushStr[0] = static_cast<char>(setCI0 + pin);
          flushStr.append(to_flushString(config));
        }
      }else{
        //ERROR
      }
    }
    serialFlush(flushStr);
    plotFlushStringToConsole(flushStr);
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
    std::string flushStr = "S V I ";
    flushStr.append(row[0]);
    flushStr.append(row[1]);
    flushStr.append(" ");
    flushStr.append(row[2]);
    serialFlush(flushStr);
//    std::cout<<flushStr<<std::endl;
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
  if(result!=NULL){
    int colCnt = mysql_num_fields(result);
    MYSQL_ROW row;
    while(row = mysql_fetch_row(result)){
      std::string flushStr = "G V ";
      flushStr.append(row[0]);
      flushStr.append(" ");
      flushStr.append(row[1]);
      flushStr.append(row[2]);
      serialFlush(flushStr);
//      std::cout<<flushStr<<std::endl;
    }
    mysql_free_result(result);
  }

}
void ArduIoInterface::test(){
  while(!mysqlcon::connect()){
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  std::cout << "now sending config once!" << std::endl;
  sendConfig(true);
}
