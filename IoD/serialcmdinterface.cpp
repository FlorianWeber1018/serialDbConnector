#include <string>
#include "serialcmdinterface.h"
#include <thread>
#include <iostream>
using namespace std;

serialCmdInterface::serialCmdInterface(string device, int baudrate)
{
	this->device=device;
	this->baudrate=baudrate;
	std::cout << "serialCmdInterface::serialCmdInterface" << std::endl;
}

serialCmdInterface::~serialCmdInterface()
{
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

bool serialCmdInterface::serialFlush(string cmdstr)
{
	if (connectionEstablished)
	{
		cmdstr += eot;
		while(bufOut.size() >= SizeBufOutMax){
			std::cout<< "BUFFER FULL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		bufOut.push_back(cmdstr);
		return false;
	}
	else
	{
		cout<<"serialFlush::noConnection"<<endl;
		return true;
	}
	return true;
}

bool serialCmdInterface::connect()
{
	if(connectionEstablished){
		disconnect();
	}
	const char *_device = device.c_str();
	handle = serialOpen(_device, baudrate);
	if(handle!=-1){
		connectionEstablished=true;
		//rtr=true;
	}else{
		connectionEstablished=false;
		//rtr=false;
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

void serialCmdInterface::serialDispatcher(string cmd)
{

	cout<<"serialDispatcherBaseClass:"<<cmd<<endl;

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
	//cout << "listen thread!" << endl;
	listenEnable=true;
	string tempIn;
	while (listenEnable) {

		char m_char = serialCmdInterface::pollOne();
//		cout << "char:" << m_char << endl;

		if(m_char == eot){

			//rtr=true;
			serialDispatcher(tempIn);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			tempIn = "";
		}else{
//			cout << "char received: " << m_char << endl;
			if( isalpha(m_char) || isdigit(m_char) || m_char==' ' ){
				tempIn += m_char;
			}
		}
	}
}

void serialCmdInterface::Sending()
{
	//cout << "send thread!"<<endl;
	sendEnable=true;
	while (sendEnable) {

		if(!bufOut.empty()){
			string temp = bufOut.front();
			//if(rtr){
				//rtr=false;
			sendOne(temp);
			std::cout << temp << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			bufOut.pop_front();
			//}
		}else{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
