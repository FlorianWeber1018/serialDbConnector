#ifndef signalRouter__h
#define signalRouter__h
#include "../IoD/mysqlcon.h"
#include "module.h"
#include <map>

class SignalRouterIn{
public:
  SignalRouterIn(
     std::string host, unsigned int port, std::string user, std::string pw,
     std::string db
  );

protected:
  mysqlcon* mySqlConnection = nullptr;
private:
}



#endif
