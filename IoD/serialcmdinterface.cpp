#include <string>
#include "serialcmdinterface.h"
#include <thread>
#include <iostream>
#include <iomanip>

serialCmdInterface::serialCmdInterface(std::string device, int baudrate)
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
		std::cout<<"run::noConnection!"<<std::endl;
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

bool serialCmdInterface::serialFlush(std::string cmdstr)
{
	if (connectionEstablished)
	{
		cmdstr += eot;
		while(bufOut.size() >= SizeBufOutMax){
			std::cout<< "BUFFER FULL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		bufOut.push_back(cmdstr);
		return false;
	}
	else
	{
		std::cout<<"serialFlush::noConnection"<<std::endl;
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

void serialCmdInterface::serialDispatcher(std::string cmd)
{

	std::cout<<"serialDispatcherBaseClass:"<<cmd<<std::endl;

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
	std::thread m_thread(&serialCmdInterface::Sending, this);
	m_thread.detach();

}
void serialCmdInterface::startListening()
{
//	cout << "here should listening thread start" << endl;
	std::thread m_thread(&serialCmdInterface::Listening, this);
	m_thread.detach();
}
void serialCmdInterface::Listening()
{
	//cout << "listen thread!" << endl;
	listenEnable=true;
	std::string tempIn;
	while (listenEnable) {

		char m_char = serialCmdInterface::pollOne();
//		cout << "char:" << m_char << endl;

		if(m_char == eot){

			//rtr=true;
			serialDispatcher(tempIn);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			tempIn = "";
		}else{
			tempIn += m_char;
		}
	}
}

void serialCmdInterface::Sending()
{
	//cout << "send thread!"<<endl;
	sendEnable=true;
	while (sendEnable) {

		if(!bufOut.empty()){
			std::string temp = bufOut.front();
			//if(rtr){
				//rtr=false;
			sendOne(temp);

			std::this_thread::sleep_for(std::chrono::milliseconds(15));
			bufOut.pop_front();
			//}
		}else{
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}
	}
}

bool serialCmdInterface::sendOne(std::string m_string)
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
		//cout << "char sended:" << m_char << endl;
		serialPutchar(handle, m_char-number0);
		return false;
	}else{
		return true;
	}
}

char serialCmdInterface::pollOne()
{
	return (char)serialGetchar(handle) + number0;// + protocoll offset
}


std::string serialCmdInterface::to_flushString(short number)
{
	std::string result = {1,1,1,1,0};  //nullterminated
	result[0] = (unsigned char)(   number & 0x000F ) + number0;
	result[1] = (unsigned char)( ( number >> 4 ) & 0x000F) + number0;
	result[2] = (unsigned char)( ( number >> 8 ) & 0x000F) + number0;
	result[3] = (unsigned char)( ( number >> 12 ) & 0x000F) + number0;
	return result;
}
std::string serialCmdInterface::to_flushString(unsigned char number)
{
	std::string result = "00";  //nullterminated
	result[0] = ( number & 0x0F ) + number0;
	result[1] = (( number >> 4 ) & 0x0F) + number0;
	return result;
}
unsigned char serialCmdInterface::to_uchar(const std::string& flushString)
{
	unsigned char result = 0;
	result |= ( flushString[0] & 0x0F ) - number0);
	result |= ( ( flushString[1] & 0x0F ) - number0) << 4;
	return result;
}
short serialCmdInterface::to_short(const std::string& flushString)
{
	short result = 0;
	result |= ( flushString[0] & 0x0F ) - number0);
	result |= ( ( flushString[1] & 0x0F ) - number0) << 4;
	result |= ( ( flushString[2] & 0x0F ) - number0) << 8;
	result |= ( ( flushString[3] & 0x0F ) - number0) << 12;
	return result;
}
void serialCmdInterface::plotFlushStringToConsole(const std::string& flushString)
{
	for (std::string::size_type i = 0; i < flushString.length(); ++i){
		  std::cout << " " << std::hex << std::setfill('0') << std::setw(2) << std::nouppercase << (unsigned char)flushString[i] - 1;
	}
}
