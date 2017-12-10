#include <iostream>
#include "ArduIoInterface.h"
#include <string>

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
  std::string sqlQuery = "Select DeviceID As dev, SignalID as PIN, config as cnf from heizung.IoConfigValue;";
  std::string resultStr = sendCommand(sqlQuery);
  std::cout << resultStr<<std::endl;
}
