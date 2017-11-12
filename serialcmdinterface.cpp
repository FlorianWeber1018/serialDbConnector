#include <string>
#include "serialcmdinterface.h"
#include <thread>
using namespace std;

serialCmdInterface::serialCmdInterface(string device, int baudrate)
{
	connect(device, baudrate);
}

serialCmdInterface::~serialCmdInterface()
{
	if(connectionEstablished){
		disconnect();
	}
}

bool serialCmdInterface::getConnectionState()
{
	return connectionEstablished;
}

bool serialCmdInterface::flush(String cmdstr)
{
	if (connectionEstablished)
	{
		cmdstr.append(eot);
		bufOut.append(cmdstr);
		return false;
	}
	else
	{
		return true;
	}
	return true;
}

bool serialCmdInterface::connect(string device, int baudrate)
{
	if(connectionEstablished){
		disconnect();
	}
	handle = serialOpen(device, baudrate);
	if(handle!=-1){
		connectionEstablished=true;
	}else{
		connectionEstablished=false;
	}
	return connectionEstablished;
}

void serialCmdInterface::disconnect()
{
	if(connectionEstablished){
		serialClose(handle);
		connectionEstablished = false;
	}
}

void serialCmdInterface::dispatcher(string cmd)
{

}
void serialCmdInterface::stopListening()
{
	listenEnable=false;
}
void serialCmdInterface::stopSending()
{
	sendEnable=false;
}
void serialCmdInterface::startSending()
{
	thread(Sending);
}
void serialCmdInterface::startListening()
{
	thread(Listening);
}
void serialCmdInterface::Listening()
{
	listenEnable=true;
	static string tempIn;
	while (listenEnable) {
		char m_char = pollOne();
		if(m_char == eot){
			bufIn.append(tempIn);
			tempIn = "";
		}else{
			tempIn.append(m_char);
		}
	}
}

void serialCmdInterface::Sending()
{
	sendEnable=true;
	while (sendEnable) {
		if(!bufOut.empty()){
			sendOne(bufOut.front());
			bufOut.pop_front();
		}
	}
}

bool serialCmdInterface::sendOne(String m_string)
{
	int i=0;
	while(m_string[i]!=NULL){
		if(sendOne(m_string[i])){
			return true;
		}else{
			i++;
		}
	}
	return false;
}

bool serialCmdInterface::sendOne(char m_char)
{
	if(connectionEstablished){
		serialPutchar(handle, m_char);
		return false;
	}else{
		return true;
	}
}

char serialCmdInterface::pollOne()
{
	return (char)serialGetchar(handle);
}
