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
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
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
		//cout << "char sended:" << m_char << endl;
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


std::string serialCmdInterface::to_flushString(short number)
{
	std::string result = {1,1,1,1,1,0};  //nullterminated
	if (number < 0){
		result[0] = minus;
		number *= -1;
	}else{
		result[0]= plus;
	}
	for (char i = 4; i >= 1; i--){
			switch(i){
				case 1:{
					result[i] = number + number0;
				}break;
				case 2:{
					result[i] = ( number >> 4 ) + number0;;
					number -= ( result[i] - number0 ) << 4;
				}break;
				case 3:{
					result[i] = ( number >> 8 ) + number0;;
					number -= ( result[i] - number0 ) << 8;
				}break;
				case 4:{
					result[i] = ( number >> 12 ) + number0;
					number -= ( result[i] - number0 ) << 12;
				}break;
			}
		}
	for(char i = 4; i >= 0; i--){
		if(result[i] != 1){
			result = result.substr(0, i+1);
		}
	}
}
std::string serialCmdInterface::to_flushString(unsigned char number)
{
	std::string result = {1,1,0};  //nullterminated
	for (char i = 1; i >= 0; i--){
		switch(i){
			case 0:{
				result[i] = number + number0;
			}break;
			case 1:{
				result[i] = (number >> 4) + number0;
				number -= ( result[i] - number0 ) << 4 ;
			}break;
		}
	}
	for(char i = 1; i >= 0; i--){
		if(result[i] != number0){
			result = result.substr(0, i+1);
		}
	}
}
unsigned char serialCmdInterface::to_uchar(const& std::string flushString)
{
	unsigned char i = 0;
	unsigned char result = 0;
	unsigned char partitialResult = flushString[i];
	while(partitialResult != 0){
		switch(i){
			case 0:{
				result += partitialResult - number0;
			}break;
			case 1:{
				result += ( partitialResult - number0 ) << 4;
			}break;
			default:{
				return 0;
			}
		}
		partitialResult = flushString[++i];
	}
	return result;
}
short serialCmdInterface::to_short(const& std::string flushString)
{
	bool negFlag=false;
	if(flushString[0] == plus){
		negFlag = false;
	}else{
		if(flushString[0] == minus){
			negFlag = true;
		}else{
			return 0;
		}
	}

	short result = 0;
	unsigned char i = 1;
	short partitialResult = flushString[i];
	while(partitialResult != 0){
		switch(i){
			case 1:{
				result += partitialResult - number0;
			}break;
			case 2:{
				result += ( partitialResult - number0 ) << 4;
			}break;
			case 3:{
				result += ( partitialResult - number0 ) << 8;
			}break;
			case 4:{
				result += ( partitialResult - number0 ) << 12;
			}break;
			default:{
				return 0;
			}
		}
		partitialResult = flushString[++i];
	}

	if(negflag){
		return 0 - result;
	}else{
		return result;
	}
}
void plotFlushStringToConsole(const& std::string flushString)
{
	for (string::size_type i = 0; i < s.length(); ++i){
		  std::cout << std::hex << std::setfill('0') << std::setw(2) << std::nouppercase << (int)flushString[i] - 1;
	}
    std::cout << std::endl;
}
