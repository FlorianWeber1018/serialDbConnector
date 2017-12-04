#include "mysqlcon.h"
#include <string>
#include <iostream>

mysqlcon::mysqlcon(std::string host, unsigned int port, std::string user, std::string pw, std::string db)
{
	this->host = host;
	this->port = port;
	this->user = user;
	this->pw = pw;
	this->db = db;
	this->connected = false;
}
mysqlcon::~mysqlcon() {
	disconnect();
}
std::string mysqlcon::connect()
{
	//initialize MYSQL object for connections
	m_mysql = mysql_init(NULL);
	std::string errorbuffer="";
	if (m_mysql == NULL)
	{
		errorbuffer.append(mysql_error(m_mysql));
		errorbuffer.append("\n");
		return errorbuffer;
	}

	//Connect to the database
	if (mysql_real_connect(m_mysql, host.c_str(), user.c_str(), pw.c_str(), db.c_str(), port, NULL, 0) == NULL)
	{
		errorbuffer.append(mysql_error(m_mysql));
		errorbuffer.append("\n");
	}
	else
	{
		this->connected = true;
	}
	return errorbuffer;
}
void mysqlcon::disconnect()
{
	mysql_close(m_mysql);
	connected = false;
}
std::string mysqlcon::sendCommand(std::string sendstring)
{
	std::string resultstringcomplete="";
	if (connected)
	{
		if (!mysql_query(m_mysql, sendstring.c_str()))
		{
			MYSQL_RES *result = mysql_store_result(m_mysql);
			if (result != NULL)
			{
				int num_fields = mysql_num_fields(result);
				MYSQL_ROW row;
				while ((row = mysql_fetch_row(result)))
				{
					for(int i = 0; i < num_fields; i++)
    					{
        					resultstringcomplete.append("#");
						resultstringcomplete.append(row[i] ? row[i] : "[NULL]");
    					}
        				resultstringcomplete.append("\n");
				}
				mysql_free_result(result);
			}
		}
	}
	return resultstringcomplete;
}
