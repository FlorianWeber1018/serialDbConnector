#ifndef __ArduIoInterface_h
#define __ArduIoInterface_f


#include <string>
#include <list>
#include "COMprotocol.h"
#include "mysqlcon.h"
#include "serialcmdinterface.h"



class ArduIoInterface : public serialCmdInterface, public mysqlcon
{
  ArduIoInterface(std::string device, int baudrate, std::string host, unsigned int port, std::string user, std::string pw, std::string db) : serialCmdInterface(device, baudrate), mysqlcon(host, port, user, pw, db);

  private:
    void serialDispatcher(std::string cmd);
    void connectBoth();

};

#endif
