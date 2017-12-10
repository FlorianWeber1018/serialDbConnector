#ifndef __mysqlcon_h
#define __mysqlcon_h
#include <string>
#include <stdlib.h>
#include <mysql/mysql.h>

class mysqlcon {

public:
	mysqlcon( std::string host, unsigned int port, std::string user, std::string pw, std::string db);
	~mysqlcon();
	bool connect();
	void disconnect();
	MYSQL_RES* sendCommand(std::string);
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
