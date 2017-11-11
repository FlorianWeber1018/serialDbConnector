#include <string>
#include "serialcmdinterface.h"
using namespace std;

serialCmdInterface::serialCmdInterface(string device, string baudrate) 
{
	this->device = device;
	this->baudrate = baudrate;
	connect();
}
serialCmdInterface::~serialCmdInterface() 
{
	disconnect();
}
bool serialCmdInterface::getConnectionState() 
{
	return connectionEstablished;
}
bool serialCmdInterface::sendCommand(string cmdstr)
{
	if (connectionEstablished) 
	{
		//////////////////////////////////////TODO
		return false;
		
	}
	else 
	{ 
		return true; 
	}
	return true;
}
bool serialCmdInterface::connect()
{

}
void serialCmdInterface::disconnect()
{

}
