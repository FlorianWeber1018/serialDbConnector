#include <string>
#include "serialcmdinterface.h"
#include <thread>
#include <iostream>
using namespace std;

serialCmdInterface::serialCmdInterface(string device, int baudrate)
{
	m_mysqlcon = new mysqlcon("192.168.178.92", 3306, "root", "637013", "heizung");
	connect(device, baudrate);
	rtr = true;
}

serialCmdInterface::~serialCmdInterface()
{
	delete m_mysqlcon;
	stopSending();
	stopListening();
	if(connectionEstablished){
		disconnect();
	}
}

void serialCmdInterface::run()
{
	if(connectionEstablished){
		startListening();
		startSending();
	}else{
		cout<<"run::noConnection!"<<endl;
	}
}
void serialCmdInterface::stop()
{
	stopSending();
	stopListening();
}

bool serialCmdInterface::getConnectionState()
{
	return connectionEstablished;
}

bool serialCmdInterface::flush(string cmdstr)
{
	if (connectionEstablished)
	{
		cmdstr += eot;
		bufOut.push_back(cmdstr);
//		cout << "buffer after flush():" << bufOut.front() << endl;
		return false;
	}
	else
	{
		cout<<"flush::noConnection"<<endl;
		return true;
	}
	return true;
}

bool serialCmdInterface::connect(string device, int baudrate)
{
	if(connectionEstablished){
		disconnect();
	}
	const char *_device = device.c_str();
	handle = serialOpen(_device, baudrate);
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
	
	cout<<"dispatch:"<<cmd<<endl;
	
	mysqlcon* m_mysqlcon = new  mysqlcon("192.168.178.92", 3306, "root", "637013", "heizung");
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
//	cout << "here should sending thread start" << endl;
	thread m_thread(&serialCmdInterface::Sending, this);
	m_thread.detach();

}
void serialCmdInterface::startListening()
{
//	cout << "here should listening thread start" << endl;
	thread m_thread(&serialCmdInterface::Listening, this);
	m_thread.detach();
}
void serialCmdInterface::Listening()
{
	cout << "listen thread!" << endl;
	listenEnable=true;
	string tempIn;
	while (listenEnable) {
		char m_char = serialCmdInterface::pollOne();
//		cout << "char!" << m_char << endl;
		if(m_char == eot){
			bufIn.push_back(tempIn);
			rtr=true;
			dispatcher(tempIn);
			tempIn = "";
		}else{
//			cout << "char received: " << m_char << endl;
			tempIn += m_char;
		}
	}
}

void serialCmdInterface::Sending()
{
	cout << "send thread!"<<endl;
	sendEnable=true;
	while (sendEnable) {
		if(!bufOut.empty()){
			string temp = bufOut.front();
			if(rtr){
				cout << "sended:" << temp << endl;
				rtr=false;
				sendOne(temp);
				bufOut.pop_front();
			}
		}
	}
}

bool serialCmdInterface::sendOne(string m_string)
{
	int i=0;
	while(m_string[i]!=0){
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
//		cout << "char sended:" << m_char << endl;
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
