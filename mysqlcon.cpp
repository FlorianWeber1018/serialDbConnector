#include "mysqlcon.h"
#include <string>
#include <iostream>
#include <mysql/mysql.h>

mysqlcon::mysqlcon(std::string host, unsigned int port, std::string user, std::string pw, std::string db)
{
	this->host = host;
	this->port = port;
	this->user = user;
	this->pw = pw;
	this->db = db;
	this->connected_senderThread = false;
	this->connected_dispatcherThread = false;
	std::cout << "mysqlcon::mysqlcon" << std::endl;
}
mysqlcon::~mysqlcon() {
	disconnect();
}
bool mysqlcon::connect()
{
	//initialize MYSQL object for connections
	m_mysql_senderThread = mysql_init(NULL);
	if (m_mysql_senderThread == NULL)
	{
		return false;
	}
	m_mysql_dispatcherThread = mysql_init(NULL);
	if (m_mysql_dispatcherThread == NULL)
	{
		return false;
	}

	//Connect to the database
	if (mysql_real_connect(m_mysql_senderThread, host.c_str(), user.c_str(), pw.c_str(), db.c_str(), port, NULL, 0) == NULL)
	{
		return false;
	}
	else
	{
		this->connected_senderThread = true;
	}
	if (mysql_real_connect(m_mysql_dispatcherThread, host.c_str(), user.c_str(), pw.c_str(), db.c_str(), port, NULL, 0) == NULL)
	{
		return false;
	}
	else
	{
		this->connected_dispatcherThread = true;
	}
	return true;
}
void mysqlcon::disconnect()
{
	if(this->connected_senderThread){
		mysql_close(m_mysql_senderThread);
		this->connected_senderThread=false;
	}
	if(this->connected_dispatcherThread){
		mysql_close(m_mysql_dispatcherThread);
		this->connected_dispatcherThread=false;
	}
}
MYSQL_RES* mysqlcon::sendCommand(std::string sendstring, MYSQL* connectionObject)
{
	if (!mysql_query(connectionObject, sendstring.c_str()))
	{
		//std::cout << "got recordset" << std::endl;
		return mysql_store_result(connectionObject);
	}
	//std::cout << "no recordset" << std::endl;
	return NULL;
}
MYSQL_RES* mysqlcon::sendCommand_senderThread(std::string sendstring)
{
	if(connected_senderThread){
		return sendCommand(sendstring, m_mysql_senderThread);
	}
	return NULL;
}
MYSQL_RES* mysqlcon::sendCommand_dispatcherThread(std::string sendstring)
{
	if(connected_dispatcherThread){
		return sendCommand(sendstring, m_mysql_dispatcherThread);
	}
	return NULL;
}
