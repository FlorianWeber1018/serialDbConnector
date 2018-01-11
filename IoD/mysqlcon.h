#ifndef __mysqlcon_h
#define __mysqlcon_h
#include <string>
#include <stdlib.h>
#include <mysql/mysql.h>

class mysqlcon {

public:
	mysqlcon(std::string host, unsigned int port, std::string user,
		std::string pw, std::string db
	);
	~mysqlcon();
	bool connect();
	void disconnect();
	MYSQL_RES* sendCommand_senderThread(std::string sendstring);
	MYSQL_RES* sendCommand_dispatcherThread(std::string sendstring);
private:
	MYSQL_RES* sendCommand(std::string, MYSQL* connectionObject);
	MYSQL* m_mysql_senderThread;
	MYSQL* m_mysql_dispatcherThread;
	bool connected_senderThread;
	bool connected_dispatcherThread;
	std::string host;
	unsigned int port;
	std::string user;
	std::string pw;
	std::string db;
};
#endif
