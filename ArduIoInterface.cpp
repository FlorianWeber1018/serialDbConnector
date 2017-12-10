#include <iostream>
#include "ArduIoInterface.h"

void ArduIoInterface::serialDispatcher(std::string cmd)
{
  std::cout << "ArduIoInterface::SerialDispatcher" << std::endl;
}
bool ArduIoInterface::connect()
{
  std::cout << "ArduIoInterface::connectBoth" << std::endl;
  return true;
}
ArduIoInterface::ArduIoInterface(std::string device, int baudrate, std::string host, unsigned int port, std::string user, std::string pw, std::string db) :
                              serialCmdInterface(device, baudrate),
                              mysqlcon(host, port, user, pw, db)
{
  std::cout << "ArduIoInterface::ArduIoInterface" << std::endl;
}
