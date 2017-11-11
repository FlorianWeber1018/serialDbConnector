#ifndef __mysqlcon_h
#define __mysqlcon_h
#include <string.h>
#include <stdlib.h>
#include <mysql.h>

class mysqlcon {

public:
	mysqlcon(MYSQL m_mysql, std::string host, unsigned int port, std::string user, std::string pw, std::string db);
	~mysqlcon();
	std::string connect();
	void disconnect();
	std::string sendCommand(std::string);
private:
	MYSQL* m_mysql;
	bool connected;
	std::string host;
	unsigned int port;
	std::string user;
	std::string pw;
	std::string db;
};


#endif